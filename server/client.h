//
// Created by iceloca on 7.05.24.
//

#ifndef SFML_CLIENT_H
#define SFML_CLIENT_H
#include "server_structures.h"
#define PORT 12344
#define SERVER_PORT 52345
int initialize_client(int port);
void initialize_server(int port,const char* server_ip, struct sockaddr_in* server_addr);
void send_client_data(client_data_t data, int sockfd, struct sockaddr_in server_addr);
void receive_game_data(send_data_t * data, int sockfd, struct sockaddr_in server_addr);

void make_non_block(int sockfd);
void make_block(int sockfd);
#endif //SFML_CLIENT_H
