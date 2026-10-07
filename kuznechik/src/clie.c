#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "kuznechik.h"
#include "network.h"

int main(void){
    u8 master_key[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };
    kuznechik_init(master_key);

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
            encrypt_message(buffer, encrypted, &enc_len);

            send_message(client_fd, encrypted, enc_len);
        }

        if(FD_ISSET(client_fd, &readfds)){
            u8 encrypted[BUFFER_SIZE];
            int bytes = recv_message(client_fd, encrypted, BUFFER_SIZE);

            if(bytes <= 0){
                rainbow_text_smooth("server disconnected\n");
                break;
            }

            char decrypted[BUFFER_SIZE];
            decrypt_message(encrypted, bytes, decrypted);

            rainbow_text_smooth(decrypted);
            printf("\n");
        }
    }

    close(client_fd);
    return 0;
}