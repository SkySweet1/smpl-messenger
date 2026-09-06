#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
#include <sys/select.h>
#include <arpa/inet.h>  

#define PORT 0002
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 2

typedef unsigned char u8;

void set_rgb_color(int r, int g, int b){
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}
/*
функция установки цвета
*/

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
/*
функция радужного вывода текста
радужный текст — функция rainbow_text_smooth() использует синусоиды для плавного перехода цветов RGB
каждый символ имеет свой цвет в зависимости от позиции в строке
*/

int main(void){
    int server_fd;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addren = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("invalid socket\n");
        exit(EXIT_FAILURE);
    }
    /*
    создание сокета
    */
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    /*
    настройка адреса
    */


    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind invalid\n");
        exit(EXIT_FAILURE);
    }
    /*
    биндинг
    */

    if(listen(server_fd, 3) < 0){
        perror("listen invalid\n");
        exit(EXIT_FAILURE);
    }
    /*
    прослушивание
    */

    rainbow_text_smooth("so far so good, it seems...\n");

    rainbow_text_smooth("----------------------------\n");

    for(int i = 0; i < MAX_CLIENTS; i++){
        client_sockets[i] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addren);
        printf("\033[90mclie %d connect\033[0m\n", i+1);

        char welcome[] = "welcome to encrypted chat!";
        uint32_t len_net = htonl(strlen(welcome));
        
        send(client_sockets[i], &len_net, 4, 0);
        send(client_sockets[i], welcome, strlen(welcome), 0);
    }
    /*
    прием двух клиентов
    */

    rainbow_text_smooth("----------------------------\n");

    while(1){
        fd_set readfds;                                 // набор файловых дексрипторов
        FD_ZERO(&readfds);                              // обнуление набора
        FD_SET(client_sockets[0], &readfds);            // добавить сокет первого клиента
        FD_SET(client_sockets[1], &readfds);            // добавить сокет второго клиента
        
        int max_fd = (client_sockets[0] > client_sockets[1]) ? client_sockets[0] : client_sockets[1];
        select(max_fd + 1, &readfds, NULL, NULL, NULL);
        /*
        определение максимального fd нужно для select
        select ждет активности на любом сокете
        */

        if(FD_ISSET(client_sockets[0], &readfds)){
        /*
        проверяем есть ли данные для чтения от первого слиента (client_sockets[0])

        это обработка входящего сообщения от 1 лиента на сервере - ретрансляторе 
        сервер получает зашифрованные данные от одного клиента и пересылает их другому клиенту без расшифровки
        сервер не знает содержимого сообщения, он просто ретранслирует шифротекст
        */
            //чтение длины сообщения
            uint32_t len_net;
            int bytes = recv(client_sockets[0], &len_net, 4, 0);
            /*
            читаем первые 4 байта - это длина защифрованного сообщения в сетевом порядке (big-endian)

            len_net - переменная для хранения длины
            bytes - количество прочитанных байт (должно быть 4)
            */
         

            if(bytes <= 0) {
                printf("\033[90mclie 1 disconnect\033[0m\n");
                continue;
            }
            /*
            если прочитано 0 байл - клиент отлючился
            если прочитанно меньше 0 байт - ошибка чтения
            выходим из обработки и ждем следующего события
            */

            size_t data_len = ntohl(len_net);
            /*
            ntonl() - преобразует длину из сетевого порядка (big-endian) в порядок хоста (little-endian на x86)
            теперь data_len - это реальная длина зашифрованных данных
            */

            // преобразование длинны
            if(data_len > BUFFER_SIZE){
                data_len = BUFFER_SIZE;
            }
            /*
            защита от переполнения:
                если длина больше буфера - образать
            */

            // чтнение зашифрованных данных
            u8 encrypted[BUFFER_SIZE];
            recv(client_sockets[0], encrypted, data_len, 0);
            /*
            читаем data_len байт зашифрованных данных в массив encrypted
            */

            // пересылка второму клиенту (длина + данные)
            send(client_sockets[1], &len_net, 4, 0);
            send(client_sockets[1], encrypted, data_len, 0);
            /*
            отправляем длину (4 байта) второму клиенту
            отправляем сами данные второму клиенту

            Сервер не расшифровывает данные - он просто ретранслирует шифротекст
            */
        }
        /*
        проверка - пришли ла данные от первого клиента
        */

        if(FD_ISSET(client_sockets[1], &readfds)){
        /*
        проверяем есть ли данные для чтения от второго слиента (client_sockets[1])

        это обработка входящего сообщения от клиента 2 на сервере - ретрансляторе
        сервер получает зашифрованные данные от второго клиента и пересылает их первому - заркально по сравнению с клиентом 1
        */

            // чтение длины сообщения
            uint32_t len_net;
            int bytes = recv(client_sockets[1], &len_net, 4, 0);
            /*
            читаем первые 4 байта - длинузашифрованного сообщения в сетевом порядке (big-endian)
            len_net - переменная для хранения длины
            bytes - количество прочитанных байт (должно быть 4)
            */

            if(bytes <= 0) {
                printf("\033[90mclie 2 disconnect\033[0m\n");
                continue;
            }
            /*
            если bytes <= 0 - клиент отключился или ошибка чтения
            выводим сообщение и выходим из обработчика, ждем следующего события
            */

            // преобразование длины
            size_t data_len = ntohl(len_net);
            /*
            ntohl() — преобразует длину из сетевого порядка (big-endian) в порядок хоста
            */

            if(data_len > BUFFER_SIZE){
                data_len = BUFFER_SIZE;
            }
            /*
            защита от переполнения: обрезаем до размера буфера, если длина больше BUFFER_SIZE
            */

            // чтение зашифрованных данных
            u8 encrypted[BUFFER_SIZE];
            recv(client_sockets[1], encrypted, data_len, 0);
            /*
            читаем data_len , зашифрованных байт в массив encrypted
            */

            // пересылка первому клиенту (длина + данные)
            send(client_sockets[0], &len_net, 4, 0);
            send(client_sockets[0], encrypted, data_len, 0);
            /*
            отправляем длину (4 байта) второму клиенту
            отправляем сами данные второму клиенту
            */
        }
        /*
        проверка - пришли ла данные от второго клиента

        Получает от клиента 2 зашифрованное сообщение:
            читает длину (4 байта)
            проверяет что клиент не отключился
            преобразует длине из сетевого порядка
            читает зашифрованные данные
            пересылает их клиенту 1
        */
    }

    close(server_fd);
    /*
    закрываем соединение
    */

    return 0;
}
