#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "server_structures.h"
#define PORT 12345
#define BUFFER_SIZE 1024
#define BORDER_MIN_SIZE_Y 64
#define BORDER_MIN_SIZE_X 64
#define BORDER_MAX_SIZE_Y (720 - BORDER_MIN_SIZE_Y)
#define BORDER_MAX_SIZE_X (1280 - BORDER_MIN_SIZE_X)
#define PLAYER_WIDTH 43
#define PLAYER_HEIGHT 64
#define BOSS_WIDTH 69
#define BOSS_HEIGHT 94
#define UPDATE_INTERVAL 6 // Интервал обновления
#define MIN_BOSS_SPEED 2.0
#define MAX_BOSS_SPEED (6.0 - MIN_BOSS_SPEED) // Максимальная скорость босса
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#define BULLET_SPEED 6.0

void func(int signum)
{
    wait(NULL);
}
long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


// Функция для проверки пересечения круга и прямоугольника
int isCircleIntersectingRectangle(struct coordinate rectCenterBottom, float rectWidth, float rectHeight, struct coordinate circleCenter, float circleRadius) {
    // Вычисляем центр прямоугольника
    struct coordinate rectCenter;
    rectCenter.x = rectCenterBottom.x;
    rectCenter.y = rectCenterBottom.y - rectHeight / 2.0;

    // Вычисляем половину ширины и высоты прямоугольника
    float halfRectWidth = rectWidth / 2.0;
    float halfRectHeight = rectHeight / 2.0;

    // Вычисляем ближайшую точку на прямоугольнике к центру круга
    double closestX = fmax(rectCenter.x - halfRectWidth, fmin(circleCenter.x, rectCenter.x + halfRectWidth));
    double closestY = fmax(rectCenter.y - halfRectHeight, fmin(circleCenter.y, rectCenter.y + halfRectHeight));

    // Вычисляем расстояние от этой точки до центра круга
    double distanceX = circleCenter.x - closestX;
    double distanceY = circleCenter.y - closestY;

    // Проверяем, меньше ли это расстояние радиуса круга
    return (distanceX * distanceX + distanceY * distanceY) < (circleRadius * circleRadius);
}

void move_boss(entity_t* boss) {
    static float angle = 0.0f;
    static float speed = 0.0f;
    static int delay_random = 0;
    static int update_interval = 0;
    static long last_rand_update_time = 0;
    // Получение текущего времени в миллисекундах
    long current_time = time(NULL);
    static long last_update_time_ms = 0;
    // Получение текущего времени в миллисекундах
    long current_time_ms = get_current_time_ms();
    // Обновление угла и скорости через определённые промежутки времени
    if (current_time - last_rand_update_time> update_interval) {
        srand(time(NULL));
        angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
        speed =MIN_BOSS_SPEED+  ((float)rand() / RAND_MAX) * MAX_BOSS_SPEED;
        update_interval = rand() % UPDATE_INTERVAL;
        last_rand_update_time = current_time;
    }
    // Вычисление новых координат босса
    if(current_time_ms - last_update_time_ms > 50) {
        boss->coordinates.x += speed * cos(angle);
        boss->coordinates.y += speed * sin(angle);
        last_update_time_ms = current_time_ms;
        // Ограничения для движения босса по границам поля
        if (boss->coordinates.x < BORDER_MIN_SIZE_X){
            double x =  - cos(angle);
            double y = sin(angle);
            angle = atan2(y, x);
            boss->coordinates.x = BORDER_MIN_SIZE_X;
        }
        if (boss->coordinates.x > BORDER_MAX_SIZE_X){
            double x =  - cos(angle);
            double y = sin(angle);
            angle = atan2(y, x);
            boss->coordinates.x = BORDER_MAX_SIZE_X;
        }
        if (boss->coordinates.y < BORDER_MIN_SIZE_Y){
            angle = -angle;
            boss->coordinates.y = BORDER_MIN_SIZE_Y;
        }
        if (boss->coordinates.y > BORDER_MAX_SIZE_Y){
            angle = -angle;
            boss->coordinates.y = BORDER_MAX_SIZE_Y;
        }
    }
}
void send_server_data(int sockfd, send_data_t send_data, struct sockaddr_in client_addr){
        if (sendto(sockfd, &send_data, sizeof(send_data), 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) ==
            -1) {
            perror("Sendto failed");
        }
        return;
}

game_data_t initialise(void) {
    game_data_t gamedata = {0};
    gamedata.boss.type = 'b';
    gamedata.boss.hp = 5;
    gamedata.boss.coordinates = (struct  coordinate){656,656};
    gamedata.player1.type = '1';
    gamedata.player1.hp = 5;
    gamedata.player1.coordinates = (struct  coordinate){256,256};
    gamedata.player2.type = '2';
    gamedata.player2.hp = 5;
    gamedata.player2.coordinates = (struct  coordinate){256,256};
    return  gamedata;
}

int bullet_empty(bullet_t bullet) {
    int var = bullet.owner == 0;
    return  var;
}
int check_hit(bullet_t bullet, entity_t entity) {
    if (entity.type == 'b' && bullet.owner !='b')
        return isCircleIntersectingRectangle(entity.coordinates,BOSS_WIDTH,BOSS_HEIGHT,bullet.coordinates,10);
    if(entity.type != 'b' && bullet.owner == 'b')
        return isCircleIntersectingRectangle(entity.coordinates,PLAYER_WIDTH,PLAYER_HEIGHT,bullet.coordinates,10);
}
//43 Ширина 64 высота
int process_bullet_hit(entity_t* entity, bullet_t bullet) {
    if ((entity->hp>0) && ((entity->type == 'b' && bullet.owner != entity->type )||
            (entity->type != 'b' && bullet.owner == 'b' ))&&
        (check_hit(bullet,* entity))) {
        entity->hp--;
        printf("Changed HP %c %d\n",entity->type, entity->hp);
        return 1;
    }
    return 0;
}
int process_hit_player(entity_t* entity, bullet_t bullet){
    if(bullet.owner != 'b')
        return 0;
    return 1;
}
int move_bullet(bullet_t* bullet) {
    bullet->coordinates.x += bullet->vector.x;
    bullet->coordinates.y += bullet->vector.y;
    if (bullet->coordinates.x < BORDER_MIN_SIZE_X || bullet->coordinates.x > BORDER_MAX_SIZE_X )
        return 1;
    if (bullet->coordinates.y < BORDER_MIN_SIZE_Y || bullet->coordinates.y > BORDER_MAX_SIZE_Y )
        return 1;
    return  0;
}
void process_bullets(game_data_t* gamedata) {
    static long last_update_time_ms = 0;
    // Получение текущего времени в миллисекундах
    long current_time_ms = get_current_time_ms();
    if(current_time_ms - last_update_time_ms < 20)
        return;
    last_update_time_ms = current_time_ms;
    int step = 0;
    int i ;
    for(i = 0; (i + step < MAX_BULLETS) && !bullet_empty(gamedata->bullets[i + step]); i ++) {
        gamedata->bullets[i] = gamedata->bullets[i+step];
       if(process_bullet_hit(&gamedata->boss, gamedata->bullets[i])||
            process_bullet_hit(&gamedata->player1,gamedata->bullets[i]) ||
            process_bullet_hit(&gamedata->player2,gamedata->bullets[i]) ||
            move_bullet(&gamedata->bullets[i+step])){
            step++;
            i--;
        }
    }
    for(; (i <MAX_BULLETS) ; i ++)
        gamedata->bullets[i] = (bullet_t){0};
    for(i = 0; (i < MAX_BULLETS) && bullet_empty(gamedata->bullets[i]); i ++)
        gamedata->bullets[i] = (bullet_t){0};
}

void push_bullet(bullet_t* bullets,bullet_t bullet) {
    int i = 0;
    while(!bullet_empty(bullets[i]))
        i++;
    if(i < MAX_BULLETS)
        bullets[i] = bullet;
}

void procces_client_data (game_data_t* gamedata, client_data_t clientdata) {
    if (clientdata.player.type == '1'){
        gamedata->player1.coordinates = clientdata.player.coordinates;
        gamedata->player1.animation = clientdata.player.animation;
        if(clientdata.heal == 1)
            gamedata->player1.hp += 1;
        if(clientdata.heal == -1)
            gamedata->player1.hp -= 1;
        if(gamedata->player1.hp > 5)
            gamedata->player1.hp = 5;

    }
    if (clientdata.player.type == '2') {
        gamedata->player2.coordinates = clientdata.player.coordinates;
        if(clientdata.heal == 1)
            gamedata->player2.hp += 1;
        if(clientdata.heal == -1)
            gamedata->player2.hp -= 1;
        if(gamedata->player2.hp > 5)
            gamedata->player2.hp = 5;

        gamedata->player2.animation = clientdata.player.animation;

    }
    printf("client hp : %d", clientdata.player.hp);
    if(!bullet_empty(clientdata.bullet)) {
        printf("Bullet %c %f %f\n",clientdata.player.type ,clientdata.bullet.vector.x, clientdata.bullet.vector.y);
        bullet_t  bullet  = clientdata.bullet;
        float x = bullet.vector.x;
        float y = bullet.vector.y;
        float distance = sqrt(x*x +y*y);
        bullet.vector.x = BULLET_SPEED  *  x /distance;
        bullet.vector.y = BULLET_SPEED  *  y / distance;
        printf("Bullet %c %f %f\n",clientdata.player.type ,bullet.vector.x, bullet.vector.y);
        push_bullet(gamedata->bullets, bullet);
    }
}

// Функция обработки снарядов

int init_server_socket(){
    int sockfd;
    struct sockaddr_in server_addr;
    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return sockfd;
}


void start_lobby(int sockfd,struct sockaddr_in* client_addr_1,struct sockaddr_in* client_addr_2) {
    struct sockaddr_in new_client_addr;
    socklen_t client_addr_len;
    char player = '0';
    char signal = 0;
    client_addr_len = sizeof (new_client_addr);
    do {
        if (recvfrom(sockfd, &signal, sizeof(signal), 0, (struct sockaddr *) &new_client_addr, &client_addr_len) ==
            -1) {
            perror("Receive error");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Signal :%c \n", signal);
        switch (signal) {
            case '0':
                break;
            case '1':
                if (player != '1' && player < '3') {
                    player += 1;
                    *client_addr_1 = new_client_addr;
                }
                break;
            case '2':
                if (player != '2' && player < '3') {
                    player += 2;

                }
                *client_addr_2 = new_client_addr;
                break;
            case 'r':
                player = '0';
                break;
            default:
                break;
        }


        if (player == '3' ) {
            signal = 's';
            int seed = time(NULL);
            if (sendto(sockfd, &signal, sizeof(signal), 0, (struct sockaddr *) client_addr_1,
                       sizeof(*client_addr_1)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

            //sleep(1);
//            if (sendto(sockfd, &seed, sizeof(seed), 0, (struct sockaddr *) client_addr_1,
//                       sizeof(*client_addr_1)) == -1) {
//                perror("Sendto failed");
//                close(sockfd);
//                exit(EXIT_FAILURE);
//            }
//            return;
            if (sendto(sockfd, &signal, sizeof(signal), 0, (struct sockaddr *) client_addr_2,
                       sizeof(*client_addr_2)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            sleep(1);
            if (sendto(sockfd, &seed, sizeof(seed), 0, (struct sockaddr *) client_addr_1,
                       sizeof(*client_addr_1)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            if (sendto(sockfd, &seed, sizeof(seed), 0, (struct sockaddr *) client_addr_2,
                       sizeof(*client_addr_2)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            return;
        }else{
            if (sendto(sockfd, &player, sizeof(player), 0, (struct sockaddr *) &new_client_addr,
                       sizeof(new_client_addr)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("Player :%c \n", player);
            signal = '0';
        }
    }while (signal != 's');

}

bullet_t shoot_bullet(game_data_t* gamedata, entity_t* shooter, entity_t* target) {
    float dx = target->coordinates.x - shooter->coordinates.x;
    float dy = target->coordinates.y - shooter->coordinates.y;
    float distance = sqrt(dx*dx + dy*dy);
    bullet_t bullet;
    bullet.coordinates.x = shooter->coordinates.x;
    bullet.coordinates.y = shooter->coordinates.y;
    bullet.vector.x = (dx / distance) * BULLET_SPEED;
    bullet.vector.y = (dy / distance) * BULLET_SPEED;
    bullet.owner = shooter->type;

    return bullet;
}

void boss_shoot_player(game_data_t* gamedata) {
    bullet_t bullet = {0};
    static long last_update_time_ms = 0;
    // Получение текущего времени в миллисекундах
    long current_time_ms = get_current_time_ms();
    if(current_time_ms - last_update_time_ms < 10000)
        return;
    last_update_time_ms = current_time_ms;
    if(gamedata->boss.hp < 0)
        return;
    if (gamedata->player1.hp > 0) {
        bullet = shoot_bullet(gamedata, &gamedata->boss, &gamedata->player1);
        push_bullet(gamedata->bullets, bullet);
    }
    if (gamedata->player2.hp > 0) {
        bullet = shoot_bullet(gamedata, &gamedata->boss, &gamedata->player2);
        push_bullet(gamedata->bullets, bullet);
    }
}

char recieve_client_data(int sockfd,game_data_t* game_data, struct sockaddr_in* client_addr, socklen_t* client_addr_len){
    client_data_t clientdata;
    size_t size = 0;
    size = recvfrom(sockfd, &clientdata, sizeof(clientdata), 0, (struct sockaddr *)client_addr, client_addr_len);
    if (size == -1) {
        if( errno != EWOULDBLOCK) {
            perror("Receive error");
        }
    }else if(size >0) {
        int success_signal = 0;
        if (sendto(sockfd, &success_signal, sizeof(success_signal), 0, (struct sockaddr *) client_addr,
                   *client_addr_len) == -1) {
            perror("Sendto failed check");
        }
        procces_client_data(game_data, clientdata);

    }
    return clientdata.player.type;
}
send_data_t make_send_data(game_data_t game_data, int number){
    send_data_t data_to_send = {0};
    data_to_send.boss = game_data.boss;
    if(number == '1') {
        printf("New hp :%d\n", game_data.player2.hp);
        data_to_send.hp = game_data.player1.hp;
        data_to_send.player = game_data.player2;
    }else {
        data_to_send.hp = game_data.player2.hp;
        data_to_send.player = game_data.player1;
    }
    for(int i = 0; i < MAX_BULLETS; i++){
        data_to_send.new_bullets[i] = game_data.bullets[i];
    }
    return data_to_send;
}



int main() {
    int sockfd;
    struct sockaddr_in client_addr, client_addr_1, client_addr_2;
    socklen_t client_addr_len = sizeof(client_addr);
    char type, player1_finish = 0, player2_finish = 0;
    game_data_t gamedata;
    sockfd = init_server_socket();
    gamedata = initialise();
    start_lobby(sockfd, &client_addr_1, &client_addr_2);
    close(sockfd);
    sockfd = init_server_socket();
    printf("Server listening on port %d...\n", PORT);
    while(1) {
        // Receive number from client
        type = recieve_client_data(sockfd, &gamedata, &client_addr, &client_addr_len);


        process_bullets(&gamedata);
        move_boss(&gamedata.boss);
        boss_shoot_player(&gamedata);


        send_server_data(sockfd, make_send_data(gamedata, type),client_addr);


    }

    // Close socket
    close(sockfd);

    return 0;
}
