
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "client.h"

int initialize_client(int port){
    int sockfd;
    int buffer_size = 128000;
    struct sockaddr_in local_addr;
    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(port);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) == -1) {
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)) == -1) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
void make_non_block(int sockfd){
    if(fcntl(sockfd,F_SETFL, O_NONBLOCK) == -1)
        perror("NON_BLOCK error");
}
void make_block(int sockfd){
    if(fcntl(sockfd,F_SETFL, 0) == -1)
        perror("NON_BLOCK error");
}

void initialize_server(int port,const char* server_ip, struct sockaddr_in* server_addr){
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr->sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
}

void    send_client_data(client_data_t data, int sockfd, struct sockaddr_in server_addr){
    int received_number;
    do{
        // Send number to server
        received_number = - 1;
        if (sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            if(errno != EAGAIN)
                perror("Sendto failed");
        }
        if (recv(sockfd, &received_number, sizeof(received_number), 0) == -1) {
            if(errno != EWOULDBLOCK) {
                perror("Check error");
            }
        } else {
            printf("Received response from server: %d\n", received_number);
        }
        usleep(10000);
    }while(received_number != 0 );
    printf("Number sent to server.\n");

    // Close socket

}

void receive_game_data(send_data_t * data, int sockfd, struct sockaddr_in server_addr){
    int count = recv(sockfd, data, sizeof(*data), 0);
    if (count == -1) {
        if(errno != EWOULDBLOCK) {
            perror("Receive error");
        }
    }else if(count >0) {
        int success_signal = 0;
        if (sendto(sockfd, &success_signal, sizeof(success_signal), 0, (struct sockaddr *) &server_addr,
                   sizeof(server_addr)) == -1) {
            perror("Sendto failed check");
        }
    }
    usleep(10000);

}