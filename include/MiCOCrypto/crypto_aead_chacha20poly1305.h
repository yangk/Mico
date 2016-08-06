/**
 ******************************************************************************
 * @file    crypto_aead_chacha20poly1305.h
 * @author  William Xu
 * @version V0.2.0
 * @date    12-Oct-2014
 * @brief   This header contains interfaces of aead chacha20 poly1305 algorithm
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


#ifndef crypto_aead_chacha20poly1305_H
#define crypto_aead_chacha20poly1305_H

#include <stddef.h>
#include "export.h"

#ifdef __cplusplus
# if __GNUC__
#  pragma GCC diagnostic ignored "-Wlong-long"
# endif
extern "C" {
#endif

#define crypto_aead_chacha20poly1305_KEYBYTES 32U
SODIUM_EXPORT
size_t crypto_aead_chacha20poly1305_keybytes(void);

#define crypto_aead_chacha20poly1305_NSECBYTES 0U
SODIUM_EXPORT
size_t crypto_aead_chacha20poly1305_nsecbytes(void);

#define crypto_aead_chacha20poly1305_NPUBBYTES 8U
SODIUM_EXPORT
size_t crypto_aead_chacha20poly1305_npubbytes(void);

#define crypto_aead_chacha20poly1305_ABYTES 16U
SODIUM_EXPORT
size_t crypto_aead_chacha20poly1305_abytes(void);

SODIUM_EXPORT
int crypto_aead_chacha20poly1305_encrypt(unsigned char *c,
                                         unsigned long long *clen,
                                         const unsigned char *m,
                                         unsigned long long mlen,
                                         const unsigned char *ad,
                                         unsigned long long adlen,
                                         const unsigned char *nsec,
                                         const unsigned char *npub,
                                         const unsigned char *k);

SODIUM_EXPORT
int crypto_aead_chacha20poly1305_decrypt(unsigned char *m,
                                         unsigned long long *mlen,
                                         unsigned char *nsec,
                                         const unsigned char *c,
                                         unsigned long long clen,
                                         const unsigned char *ad,
                                         unsigned long long adlen,
                                         const unsigned char *npub,
                                         const unsigned char *k);
#ifdef __cplusplus
}
#endif

#endif

