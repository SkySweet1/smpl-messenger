#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void rainbow_text_smooth(const char *text);
/*
Радужный вывод
*/

int send_message(int fd, const void *data, size_t len);
/*
Отправка сообщения: сначала 4 байта длины, потом данные*/

int recv_message(int fd, void *buffer, size_t max_len);
/*
Приём сообщения: сначала 4 байта длины, потом данные
Возвращает количество прочитанных байт, или -1 при ошибке
*/

#endif