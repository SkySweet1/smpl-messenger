#ifndef KUZNECHIK_H
#define KUZNECHIK_H

#include <stddef.h>

typedef unsigned char u8;

void kuznechik_init(const u8 *master_key);
/*
Инициализация (развёртка ключа)
*/

void kuznechik_encrypt(const u8 *plain, u8 *cipher);
void kuznechik_decrypt(const u8 *cipher, u8 *plain);
/*
Шифрование/расшифрование одного блока (16 байт)
*/

void encrypt_message(const char *msg, u8 *encrypted, size_t *enc_len);
void decrypt_message(const u8 *encrypted, size_t enc_len, char *decrypted);
/*
Обёртки для сообщений произвольной длины
*/

#endif