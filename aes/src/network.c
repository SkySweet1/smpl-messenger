#include "network.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

void set_rgb_color(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void rainbow_text_smooth(const char *text){
    int length = strlen(text);

    if(length == 0){
        return;
    }
    // добавленно: 
    //     добалвенна проверка на пустую строку, чтобы избежать деления на ноль при вычислении p = (float)i / length
    //     если длина строки равна нулю, то функция просто возвращает управление без вывода текста 

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

int send_message(int fd, const void *data, size_t len){
    uint32_t len_net = htonl((uint32_t)len);
    
    if(send(fd, &len_net, 4, 0) <= 0) {
        return -1;
    }
    if(send(fd, data, len, 0) <= 0) {
        return -1;
    }

    return (int)len;
}

int recv_message(int fd, void *buffer, size_t max_len){
    uint32_t len_net;

    if(recv(fd, &len_net, 4, 0) <= 0) {
        return -1;
    }
    
    size_t data_len = ntohl(len_net);

    if(data_len > max_len){
        data_len = max_len;
    }
    
    int bytes = recv(fd, buffer, data_len, 0);

    return bytes;
}