//
// Created by iceloca on 28.03.24.
//

#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_BULLETS 12
struct  coordinate {
    float x;
    float y;
};

struct bullet {
    struct coordinate coordinates;
    struct coordinate vector;
    char owner;
} typedef bullet_t;

struct entity {
    struct coordinate coordinates;
    int8_t hp;
    char type;
} typedef entity_t;

struct game_data {
    entity_t boss;
    entity_t player1;
    entity_t player2;
    bullet_t bullets[MAX_BULLETS * 10];
}typedef  game_data_t;

struct send_data {
    entity_t boss;
    entity_t player;
    int bullets_count;
    bullet_t new_bullets[MAX_BULLETS];
}typedef  send_data_t;

struct client_data {
    entity_t player;
    bullet_t bullet;
}typedef  client_data_t;

#endif //STRUCTURES_H