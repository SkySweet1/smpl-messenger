#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <stdint.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void rainbow_text_smooth(const char *text);
int send_message(int fd, const void *data, size_t len);
int recv_message(int fd, void *buffer, size_t max_len);

#endif