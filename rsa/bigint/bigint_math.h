#ifndef BIGINT_MATH_H
#define BIGINT_MATH_H

#include "bigint.h"

void bigint_mod(bigint_t *result, const bigint_t *A, const bigint_t *B);
// остаток от деления А на В

void bigint_mod_exp(bigint_t *result, const bigint_t *base, const bigint_t *exp, const bigint_t *mod);
// возведение в степень по модулю: (base ^ exp) mod mod

#endif