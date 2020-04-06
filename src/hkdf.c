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

#include "hkdf.h"

#include <bc-crypto-base/bc-crypto-base.h>

#include <assert.h>
#include <string.h>

void hkdf_sha256(uint8_t *okm, size_t okm_size,
		 const uint8_t *s, size_t ssize,
		 const uint8_t *k, size_t ksize,
		 const uint8_t *info, size_t isize)
{
    uint8_t prk[SHA256_DIGEST_LENGTH];
    uint8_t t[SHA256_DIGEST_LENGTH];
	HMAC_SHA256_CTX ctx;
	unsigned char c;

	assert(okm_size < 255 * sizeof(t));

	/* RFC 5869:
	 *
	 * 2.2.  Step 1: Extract
	 *
	 *   HKDF-Extract(salt, IKM) -> PRK
	 *
	 *    Options:
	 *       Hash     a hash function; HashLen denotes the length of the
	 *                hash function output in octets
	 *
	 *    Inputs:
	 *       salt     optional salt value (a non-secret random value);
	 *                if not provided, it is set to a string of HashLen zeros.
	 *       IKM      input keying material
	 *
	 *    Output:
	 *       PRK      a pseudorandom key (of HashLen octets)
	 *
	 *    The output PRK is calculated as follows:
	 *
	 *    PRK = HMAC-Hash(salt, IKM)
	 */
    if(s == NULL || ssize == 0) {
        uint8_t a[SHA256_DIGEST_LENGTH];
        memset(a, 0, SHA256_DIGEST_LENGTH);
        hmac_sha256(a, SHA256_DIGEST_LENGTH, k, ksize, prk);
    } else {
        hmac_sha256(s, ssize, k, ksize, prk);
    }

	/*
	 * 2.3.  Step 2: Expand
	 *
	 *    HKDF-Expand(PRK, info, L) -> OKM
	 *
	 *    Options:
	 *       Hash     a hash function; HashLen denotes the length of the
	 *                hash function output in octets
	 *
	 *    Inputs:
	 *       PRK      a pseudorandom key of at least HashLen octets
	 *                (usually, the output from the extract step)
	 *       info     optional context and application specific information
	 *                (can be a zero-length string)
	 *       L        length of output keying material in octets
	 *                (<= 255*HashLen)
	 *
	 *    Output:
	 *       OKM      output keying material (of L octets)
	 *
	 *    The output OKM is calculated as follows:
	 *
	 *    N = ceil(L/HashLen)
	 *    T = T(1) | T(2) | T(3) | ... | T(N)
	 *    OKM = first L octets of T
	 *
	 *    where:
	 *    T(0) = empty string (zero length)
	 *    T(1) = HMAC-Hash(PRK, T(0) | info | 0x01)
	 *    T(2) = HMAC-Hash(PRK, T(1) | info | 0x02)
	 *    T(3) = HMAC-Hash(PRK, T(2) | info | 0x03)
	 *    ...
	 *
	 *    (where the constant concatenated to the end of each T(n) is a
	 *    single octet.)
	 */
	c = 1;
	hmac_sha256_Init(&ctx, prk, sizeof(prk));
	hmac_sha256_Update(&ctx, info, isize);
	hmac_sha256_Update(&ctx, &c, 1);
	hmac_sha256_Final(&ctx, t);

	while (okm_size > sizeof(t)) {
		memcpy(okm, &t, sizeof(t));
		okm = okm + sizeof(t);
		okm_size -= sizeof(t);

		c++;
		hmac_sha256_Init(&ctx, prk, sizeof(prk));
		hmac_sha256_Update(&ctx, t, sizeof(t));
		hmac_sha256_Update(&ctx, info, isize);
		hmac_sha256_Update(&ctx, &c, 1);
		hmac_sha256_Final(&ctx, t);
	}
	memcpy(okm, &t, okm_size);
}