#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 2

typedef unsigned char u8;

void set_rgb_color(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void rainbow_text_smooth(const char *text){
    int length = strlen(text);
    for(int i = 0; i < length; i++){
        float p = (float)i / length;
        int r = (int)(sin(p * 2 * M_PI + 0) * 127 + 128);
        int g = (int)(sin(p * 2 * M_PI + 2 * M_PI / 3) * 127 + 128);
        int b = (int)(sin(p * 2 * M_PI + 4 * M_PI / 3) * 127 + 128);
        set_rgb_color(r, g, b);
        printf("%c", text[i]);
    }
    printf("\033[0m");
}

int main(void){
    int server_fd;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket invalid");
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
        perror("listen ionvalid");
        exit(EXIT_FAILURE);
    }

    rainbow_text_smooth("server on\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        client_sockets[i] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        printf("\033[90mClient %d connected\033[0m\n", i + 1);
    }

    while(1){
        fd_set readfds;
        FD_ZERO(&readfds);

        int max_fd = -1;

        if(client_sockets[0] != -1){
            FD_SET(client_sockets[0], &readfds);

            if(client_sockets[0] > max_fd){
                max_fd = client_sockets[0];
            }

            if(client_sockets[1] != -1){
                FD_SET(client_sockets[1], &readfds);

                if(client_sockets[1] > max_fd){
                    max_fd = client_sockets[1];
                }
            }

            if(max_fd == -1){
                break;
            }

            select(max_fd + 1, &readfds, NULL, NULL, NULL);

            if(FD_ISSET(client_sockets[0], &readfds)){
                uint32_t len_net;
                int bytes = recv(client_sockets[0], &len_net, 4, 0);

                if(bytes <= 0){
                    printf("\033[90mclie 1 disconnected\033[0m\n");
                    break;
                }

                size_t data_len = ntohl(len_net);

                if(data_len > BUFFER_SIZE){
                    data_len = BUFFER_SIZE;
                }

                u8 encrypted[BUFFER_SIZE];

                recv(client_sockets[0], encrypted, data_len, 0);

                send(client_sockets[1], &len_net, 4, 0);
                send(client_sockets[1], encrypted, data_len, 0);
            }

            if(FD_ISSET(client_sockets[1], &readfds)){
                uint32_t len_net;
                int bytes = recv(client_sockets[1], &len_net, 4, 0);

                if(bytes <= 0){
                    printf("\033[90mclie 2 disconnected\033[0m\n");
                    break;
                }

                size_t data_len = ntohl(len_net);

                if(data_len > BUFFER_SIZE){
                    data_len = BUFFER_SIZE;
                }

                u8 encrypted[BUFFER_SIZE];

                recv(client_sockets[1], encrypted, data_len, 0);

                send(client_sockets[0], &len_net, 4, 0);
                send(client_sockets[0], encrypted, data_len, 0);
            }
        }
    }

    return 0;

}