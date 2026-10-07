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

int send_message(int fd, const void *data, size_t len){
    uint32_t len_net = htonl(len);

    if(send(fd, &len_net, 4, 0) <= 0) return -1;
    if(send(fd, data, len, 0) <= 0) return -1;

    return (int)len;
}

int recv_message(int fd, void *buffer, size_t max_len){
    uint32_t len_net;

    int bytes = recv(fd, &len_net, 4, 0);
    if(bytes <= 0) return -1;

    size_t data_len = ntohl(len_net);
    if(data_len > max_len) data_len = max_len;

    bytes = recv(fd, buffer, data_len, 0);
    if(bytes <= 0) return -1;

    return bytes;
}