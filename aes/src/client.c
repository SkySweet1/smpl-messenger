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

typedef unsigned char u8;

void aes_encrypt_message(){

}

void aes_decrypt_message(){
    
}

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
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        rainbow_text_smooth("socket invalid\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
        rainbow_text_smooth("invalid address\n");
        return -1;
    }

    if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
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
            buffer[strcspn(buffer, "\n")] = 0;

            u8 encrypted[BUFFER_SIZE];
            size_t enc_len;

            aes_encrypt_message();

            uint32_t len_net = htonl(enc_len);
            send(client_fd, &len_net, 4, 0);
            send(client_fd, encrypted, enc_len, 0);
        }

        if(FD_ISSET(client_fd, &readfds)){
            uint32_t len_net;
            int bytes = recv(client_fd, &len_net, 4, 0);

            if(bytes <= 0){
                rainbow_text_smooth("server disconnected\n");
                break;
            }

            size_t enc_len = ntohl(len_net);

            if(enc_len > BUFFER_SIZE){
                enc_len = BUFFER_SIZE;
            }

            u8 encrypted[BUFFER_SIZE];
            bytes = recv(client_fd, encrypted, enc_len, 0);

            if(bytes <= 0){
                break;
            }

            char decrypted[BUFFER_SIZE];

            aes_decrypt_message();

            rainbow_text_smooth(decrypted);
            printf("\n");
        }

    }

    close(client_fd);
    return 0;
}