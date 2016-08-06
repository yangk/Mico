/**
 ******************************************************************************
 * @file    crypto_sign_ed25519.h
 * @author  William Xu
 * @version V0.2.0
 * @date    12-Oct-2014
 * @brief   This header contains interfaces of ed25519 algorithm
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */

#ifndef crypto_sign_ed25519_H
#define crypto_sign_ed25519_H

#include <stddef.h>
#include "export.h"

#ifdef __cplusplus
# if __GNUC__
#  pragma GCC diagnostic ignored "-Wlong-long"
# endif
extern "C" {
#endif

#define crypto_sign_ed25519_BYTES 64U
SODIUM_EXPORT
size_t crypto_sign_ed25519_bytes(void);

#define crypto_sign_ed25519_SEEDBYTES 32U
SODIUM_EXPORT
size_t crypto_sign_ed25519_seedbytes(void);

#define crypto_sign_ed25519_PUBLICKEYBYTES 32U
SODIUM_EXPORT
size_t crypto_sign_ed25519_publickeybytes(void);

#define crypto_sign_ed25519_SECRETKEYBYTES (32U + 32U)
SODIUM_EXPORT
size_t crypto_sign_ed25519_secretkeybytes(void);

SODIUM_EXPORT
int crypto_sign_ed25519(unsigned char *sm, unsigned long long *smlen,
                        const unsigned char *m, unsigned long long mlen,
                        const unsigned char *sk);

SODIUM_EXPORT
int crypto_sign_ed25519_open(unsigned char *m, unsigned long long *mlen,
                             const unsigned char *sm, unsigned long long smlen,
                             const unsigned char *pk);

SODIUM_EXPORT
int crypto_sign_ed25519_keypair(unsigned char *pk, unsigned char *sk);

SODIUM_EXPORT
int crypto_sign_ed25519_seed_keypair(unsigned char *pk, unsigned char *sk,
                                     const unsigned char *seed);

#ifdef __cplusplus
}
#endif

#endif

