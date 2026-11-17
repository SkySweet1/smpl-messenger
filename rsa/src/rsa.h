#ifndef RSA_H
#define RSA_H

#include "bigint.h"

typedef struct
{
    bigint_t n; // модуль (p * q)
    bigint_t e; // открытая экспонента (65537)
} rsa_public_key_t;

typedef struct
{
    bigint_t n; // модуль
    bigint_t d; // закрытая экспонента
} rsa_private_key_t;

/*
тут позже появятся функции генерации ключей и шифрования
void rsa_generate_keys(rsa_public_key_t *pub, rsa_private_key_t *priv);
*/

#endif