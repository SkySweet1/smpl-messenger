#ifndef AES_H
#define AES_H

#include <stddef.h>

typedef unsigned char u8;

void aes_init(const u8 *key);

void aes_encrypt_block(const u8 *plain, u8 *cipher);
void aes_decrypt_block(const u8 *cipher, u8 *plain);

void aes_encrypt_message(const char *msg, u8 *encrypted, size_t *enc_len);
void aes_decrypt_message(const u8 *encrypted, size_t enc_len, char *msg);

#endif