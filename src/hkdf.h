#ifndef CCAN_CRYPTO_HKDF_SHA256_H
#define CCAN_CRYPTO_HKDF_SHA256_H

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdlib.h>

/**
 * hkdf_sha256 - generate a derived key
 * @okm: where to output the key
 * @okm_size: the number of bytes pointed to by @okm (must be less than 255*32)
 * @s: salt
 * @ssize: the number of bytes pointed to by @s
 * @k: pointer to input key
 * @ksize: the number of bytes pointed to by @k
 * @info: pointer to info
 * @isize: the number of bytes pointed to by @info
 */
void hkdf_sha256(uint8_t *okm, size_t okm_size,
		 const uint8_t *s, size_t ssize,
		 const uint8_t *k, size_t ksize,
		 const uint8_t *info, size_t isize);
#endif /* CCAN_CRYPTO_HKDF_SHA256_H */
