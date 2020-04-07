//
//  random.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <assert.h>
#include <string.h>

#include <bc-crypto-base/bc-crypto-base.h>

#include "random.h"
#include "randombytes.h"
#include "hkdf.h"

void crypto_random(uint8_t* buf, size_t n) {
    assert(randombytes(buf, n) == 0);
}

static uint8_t deterministic_seed[SHA256_DIGEST_LENGTH];
static uint64_t deterministic_salt = 0;

void seed_deterministic_string(const char* string) {
    uint8_t digest[SHA256_DIGEST_LENGTH];
    sha256_Raw((uint8_t*)string, strlen(string), deterministic_seed);
    deterministic_salt = 0;
}

void deterministic_random(uint8_t* buf, size_t n) {
    deterministic_salt += 1;
    hkdf_sha256(buf, n,
    (uint8_t*)&deterministic_salt, sizeof(deterministic_salt),
    deterministic_seed, SHA256_DIGEST_LENGTH,
    NULL, 0);
}
