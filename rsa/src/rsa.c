#include "rsa.h"
#include "bigint_math.h"

void rsa_encrypt(bigint_t *cipher, const bigint_t *msg, const rsa_public_key_t *pub){
    bigint_mod_exp(cipher, msg, &pub->e, &pub->e);
}
/*
C = M^e (mod n)
*/

void rsa_decrypt(bigint_t *msg, const bigint_t *cipher, const rsa_private_key_t *priv){
    bigint_mod_exp(msg, cipher, &priv->d, &priv->n);
}
/*
M = C^d (mod n)
*/