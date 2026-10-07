#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "network.h"

#define MAX_CLIENTS 2

typedef unsigned char u8;

int main(void){
    int server_fd;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addren = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("invalid socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind invalid");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen invalid");
        exit(EXIT_FAILURE);
    }

    rainbow_text_smooth("so far so good, it seems...\n");
    rainbow_text_smooth("----------------------------\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        client_sockets[i] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addren);
        printf("\033[90mclie %d connect\033[0m\n", i+1);
    }

    rainbow_text_smooth("----------------------------\n");

    while(1){
        fd_set readfds;
        FD_ZERO(&readfds);

        int max_fd = -1;

        if(client_sockets[0] != -1){
            FD_SET(client_sockets[0], &readfds);
            if(client_sockets[0] > max_fd) max_fd = client_sockets[0];
        }
        if(client_sockets[1] != -1){
            FD_SET(client_sockets[1], &readfds);
            if(client_sockets[1] > max_fd) max_fd = client_sockets[1];
        }

        if(max_fd == -1) break;

        select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(client_sockets[0], &readfds)){
            u8 encrypted[BUFFER_SIZE];
            int bytes = recv_message(client_sockets[0], encrypted, BUFFER_SIZE);

            if(bytes <= 0){
                printf("\033[90mclie 1 disconnect\033[0m\n");
                break;
            }

            send_message(client_sockets[1], encrypted, bytes);
        }

        if(FD_ISSET(client_sockets[1], &readfds)){
            u8 encrypted[BUFFER_SIZE];
            int bytes = recv_message(client_sockets[1], encrypted, BUFFER_SIZE);

            if(bytes <= 0){
                printf("\033[90mclie 2 disconnect\033[0m\n");
                break;
            }

            send_message(client_sockets[0], encrypted, bytes);
        }
    }

    close(server_fd);
    return 0;
}