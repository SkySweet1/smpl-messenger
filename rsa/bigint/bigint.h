#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>
#include <stddef.h>

#define BIGINT_LIMBS 128
/*
128 слов по 32 бита это 4096 бит - хватает с запасом для RSA-128 и RSA-4096
*/

typedef struct {
    uint32_t limbs[BIGINT_LIMBS];

    int length;

} bigint_t;
/*
uint32_t limbs[BIGINT_LIMBS]; - массив слов, в котором хранится число в виде little-endian (младшие слова идут первыми)

справка -> little-endian - младшие слова идут первыми, старшие - последними. 
Например, число 0x12345678 будет храниться в массиве limbs как {0x5678, 0x1234, 0, 0, ...}

int length; - количество слов, которые реально используются для хранения числа. 
Например, если число занимает 3 слова, то length = 3, а оставшиеся слова в массиве limbs будут равны 0.

bigint_t - структура, которая представляет большое целое число (big integer) с фиксированным количеством слов (limbs)
*/

void bigint_init(bigint_t *number);
void bigint_from_int(bigint_t *number, uint64_t value);
void bigint_copy(bigint_t *dest, const bigint_t *src);
void bigint_print_hex(const bigint_t *number);

int bigint_cmp(const bigint_t *number_A, const bigint_t *number_b);
int bigint_is_zero(const bigint_t *number);

void bigint_add(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B);
void bigint_sub(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B);
void bigint_mul(bigint_t *result, const bigint_t *number_A, const bigint_t *number_B);

void bigint_shl(bigint_t *number, int bits);
void bigint_shr(bigint_t *number, int bits);

int bigint_get_bit(const bigint_t *number, int bit);
void bigint_print_hex(const bigint_t *number);

#endif