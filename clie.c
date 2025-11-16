#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 0002
#define BUFFER_SIZE 1024

void set_rgb_color(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void rainbow_text_smooth(const char* text){
    int length = 0;
    while(text[length] != '\0') length++;
    
    for(int i = 0; i < length; i++) {
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
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    if((client_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        rainbow_text_smooth("socket invalid\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1", &server_addr.sin_addr) <= 0){
        rainbow_text_smooth("invalid address\n");
        return -1;
    }

    if(connect(client_fd,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        rainbow_text_smooth("invalid connect\n");
        return -1;
    }

    rainbow_text_smooth("connect\n");

    while(1){
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(client_fd, &readfds);
        FD_SET(0, &readfds);

        select(client_fd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(0, &readfds)){
            fgets(buffer, BUFFER_SIZE, stdin);
            send(client_fd, buffer, strlen(buffer), 0);
        }

        if(FD_ISSET(client_fd, &readfds)){
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

            if(bytes_received > 0){
                buffer[bytes_received] = '\0';
                printf("\033[92m%s\033[0m", buffer);

            } else if(bytes_received == 0){
                rainbow_text_smooth("no connect -> server disconnected\n");
                break;

            } else {
                rainbow_text_smooth("connection error\n");
                break;

            }
        }                
    }

    close(client_fd);
    return 0;
}
