#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
#include <sys/select.h>

#define PORT 0002
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 2

void set_rgb_color(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void rainbow_text_smooth(const char* text){
    int length = 0;
    while(text[length] != '\0') length++;
    
    for(int i = 0; i < length; i++){
        float position = (float)i / length;
        
        int r = (int)(sin(position * 2 * M_PI + 0) * 127 + 128);
        int g = (int)(sin(position * 2 * M_PI + 2 * M_PI / 3) * 127 + 128);
        int b = (int)(sin(position * 2 * M_PI + 4 * M_PI / 3) * 127 + 128);
        
        set_rgb_color(r, g, b);
        printf("%c", text[i]);
    }
    printf("\033[0m"); 
}

int main(void){
    int server_fd, new_socket;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addren = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("invalid socket\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind invalid\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen invalid\n");
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
        FD_SET(client_sockets[0], &readfds);
        FD_SET(client_sockets[1], &readfds);
        
        int max_fd = (client_sockets[0] > client_sockets[1]) ? client_sockets[0] : client_sockets[1];
        
        select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(client_sockets[0], &readfds)){
            int bytes_read = recv(client_sockets[0], buffer, sizeof(buffer), 0);

            if(bytes_read > 0){
                buffer[bytes_read] = '\0';

                printf("\033[90mClient 1: %s\033[0m", buffer);

                send(client_sockets[1], buffer, bytes_read, 0);
            }
        }

        if(FD_ISSET(client_sockets[1], &readfds)){
            int bytes_read = recv(client_sockets[1], buffer, sizeof(buffer), 0);

            if(bytes_read > 0){
                buffer[bytes_read] = '\0';

                printf("\033[90mClient 2: %s\033[0m", buffer);
                send(client_sockets[0], buffer, bytes_read, 0);
            }
        }
    }

    close(server_fd);
    close(new_socket);

    return 0;
}
