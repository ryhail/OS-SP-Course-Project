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
#define BORDER_MIN_SIZE_Y 1028
#define BORDER_MIN_SIZE_X 1920
#define BORDER_MAX_SIZE_Y 1028
#define BORDER_MAX_SIZE_X 1920
#define UPDATE_INTERVAL 6 // Интервал обновления
#define MAX_BOSS_SPEED 3.0 // Максимальная скорость босса
#include <sys/time.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#define BULLET_SPEED 4.0

void func(int signum)
{
    wait(NULL);
}
long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
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
        speed = ((float)rand() / RAND_MAX) * MAX_BOSS_SPEED;
        update_interval = rand() % UPDATE_INTERVAL;
        last_rand_update_time = current_time;
    }
    // Вычисление новых координат босса
    if(current_time_ms - last_update_time_ms > 50) {
        boss->coordinates.x += speed * cos(angle);
        boss->coordinates.y += speed * sin(angle);
        last_update_time_ms = current_time_ms;
        // Ограничения для движения босса по границам поля
        if (boss->coordinates.x < 0) boss->coordinates.x = 0;
        if (boss->coordinates.x > BORDER_MAX_SIZE_X) boss->coordinates.x = BORDER_MAX_SIZE_X;
        if (boss->coordinates.y < 0) boss->coordinates.y = 0;
        if (boss->coordinates.y > BORDER_MAX_SIZE_Y) boss->coordinates.y = BORDER_MAX_SIZE_Y;
    }
}
void send_server_data(int sockfd, send_data_t send_data, struct sockaddr_in client_addr){
//    if (sendto(sockfd, &send_data, sizeof(send_data), 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) ==
//        -1) {
//        perror("Sendto failed");
//        //printf("%d", errno);
//    }
//    return;
    int received_number;
    do{
        // Send number to server
        received_number = - 1;
        if (sendto(sockfd, &send_data, sizeof(send_data), 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) ==
            -1) {
            perror("Sendto failed player");
            printf("%d", errno);
        }
        if (recv(sockfd, &received_number, sizeof(received_number),0) == -1) {
            if(errno != EWOULDBLOCK) {
                perror("Check error");
            }
        } else {
        }
    }while(received_number != 0 );
    exit(0);
}

game_data_t initialise(void) {
    //signal(SIGCHLD, func);
    game_data_t gamedata = {0};
    gamedata.boss.type = 'b';
    gamedata.boss.coordinates = (struct  coordinate){256,256};
    gamedata.player1.type = '1';
    gamedata.player1.coordinates = (struct coordinate){0,0};
    gamedata.player2.type = '1';
    gamedata.player2.coordinates = (struct coordinate){0,0};
    return  gamedata;
}

int bullet_empty(bullet_t bullet) {
    int var = bullet.owner == 0;
    return  var;
}
int check_hit(bullet_t bullet, entity_t entity, int radius) {
    float distanse = 0;
    int dif_x = bullet.coordinates.x - entity.coordinates.x;
    int dif_y = bullet.coordinates.y - entity.coordinates.y;
    distanse = sqrt(pow(dif_x,2) + pow(dif_y,2));
    return distanse < radius;
}
//43 Ширина 64 высота
int process_bullet_hit(entity_t* entity, bullet_t bullet) {
    if ((entity->hp>0) && (bullet.owner != entity->type) && (check_hit(bullet,* entity,   15))) {
        entity->hp--;
        return 1;
    }
    return 0;
}
int move_bullet(bullet_t* bullet) {
    bullet->coordinates.x += bullet->vector.x;
    bullet->coordinates.y += bullet->vector.y;
    if (bullet->coordinates.x < 0 || bullet->coordinates.x < BORDER_MAX_SIZE_X )
        return 1;
    if (bullet->coordinates.y < 0 || bullet->coordinates.y < BORDER_MAX_SIZE_Y )
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
        if (gamedata->bullets[i + step].owner != 'b') {
            printf("%c", gamedata->bullets[i+ step].owner);
            step += process_bullet_hit(&gamedata->boss, gamedata->bullets[i+step]);
            i--;
        }
        else if(process_bullet_hit(&gamedata->player1,gamedata->bullets[i+step]) ||
                process_bullet_hit(&gamedata->player2,gamedata->bullets[i+step]) ||
                move_bullet(&gamedata->bullets[i+step])){
            step++;
            i--;
        }
    }
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
    if (clientdata.player.type == '1')
        gamedata->player1.coordinates = clientdata.player.coordinates;
    if (clientdata.player.type == '2')
        gamedata->player2.coordinates = clientdata.player.coordinates;
    if(!bullet_empty(clientdata.bullet))
        push_bullet(gamedata->bullets,clientdata.bullet);
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


        if (player == '1' ) {
            signal = 's';
            int seed = time(NULL);
            if (sendto(sockfd, &signal, sizeof(signal), 0, (struct sockaddr *) client_addr_1,
                       sizeof(*client_addr_1)) == -1) {
                perror("Sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }

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

    bullet_t bullet;
    bullet.coordinates.x = shooter->coordinates.x;
    bullet.coordinates.y = shooter->coordinates.y;
    bullet.vector.x = (dx / (dx+dy)) * BULLET_SPEED;
    bullet.vector.y = (dy / (dx+dy)) * BULLET_SPEED;
    bullet.owner = shooter->type;

    return bullet;
}

void boss_shoot_player(game_data_t* gamedata, bullet_t* new_bullets) {
    bullet_t bullet = {0};
    if(gamedata->boss.hp < 0)
        return;
    if (gamedata->player1.hp > 0) {
        shoot_bullet(gamedata, &gamedata->boss, &gamedata->player1);
        push_bullet(gamedata->bullets, bullet);
        push_bullet(new_bullets, bullet);
    }
    if (gamedata->player2.hp > 0) {
        shoot_bullet(gamedata, &gamedata->boss, &gamedata->player2);
        push_bullet(gamedata->bullets, bullet);
        push_bullet(new_bullets, bullet);
    }
}

void recieve_client_data(int sockfd,game_data_t* game_data){
    client_data_t clientdata;
    size_t size = 0;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    size = recvfrom(sockfd, &clientdata, sizeof(clientdata), 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (size == -1) {
        if( errno != EWOULDBLOCK) {
            perror("Receive error");
        }
    }else if(size >0) {
        int success_signal = 0;
        if (sendto(sockfd, &success_signal, sizeof(success_signal), 0, (struct sockaddr *) &client_addr,
                   client_addr_len) == -1) {
            perror("Sendto failed check");
        }
        procces_client_data(game_data, clientdata);
        printf("%f %f", clientdata.player.coordinates.x, clientdata.player.coordinates.y);
    }
}
send_data_t make_send_data(game_data_t game_data, int number, bullet_t new_bullets [MAX_BULLETS]){
    send_data_t data_to_send = {0};
    data_to_send.boss = game_data.boss;
    if(number == 1)
        data_to_send.player= game_data.player2;
    else
        data_to_send.player= game_data.player1;
    for(int i = 0; i < MAX_BULLETS; i++){
        bullet_t bullet = new_bullets[i];
        if((bullet.owner != '0' + number) && (bullet.owner != 0)) {
            push_bullet(new_bullets, bullet);
            data_to_send.bullets_count++;
        }
    }
    return data_to_send;
}


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr, client_addr_1, client_addr_2;
    socklen_t client_addr_len = sizeof(client_addr);
    game_data_t gamedata;
    client_data_t clientdata;
    size_t size = 0;
    bullet_t  new_bullets[MAX_BULLETS] = {0};
    sockfd = init_server_socket();
    gamedata = initialise();
    start_lobby(sockfd, &client_addr_1, &client_addr_2);
    //if(fcntl(sockfd,F_SETFL, O_NONBLOCK) == -1)
    //    perror("NON_BLOCK error");
    printf("Server listening on port %d...\n", PORT);
    //sleep(2);
    while(1) {
        // Receive number from client
        recieve_client_data(sockfd, &gamedata);
        recieve_client_data(sockfd, &gamedata);
        printf(" %f , %f \n", clientdata.player.coordinates.x, clientdata.player.coordinates.y);
        process_bullets(&gamedata);
        move_boss(&gamedata.boss);
        boss_shoot_player(&gamedata, new_bullets);
        //int pid = fork();
        //if(pid==0)
        send_server_data(sockfd, make_send_data(gamedata, 1, new_bullets),client_addr_1);
        send_server_data(sockfd,make_send_data(gamedata, 2, new_bullets),client_addr_2);
        continue;
        printf(" %f , %f \n", gamedata.player2.coordinates.x, gamedata.player2.coordinates.y);


    }

    // Close socket
    close(sockfd);

    return 0;
}
