//
//  random.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <assert.h>
#include <string.h>

#include <bc-crypto-base/bc-crypto-base.h>

#include "random.hpp"
#include "randombytes.h"
#include "hkdf.h"
#include "utils.hpp"

void crypto_random(uint8_t* buf, size_t n) {
    assert(randombytes(buf, n) == 0);
}

static uint8_t deterministic_seed[SHA256_DIGEST_LENGTH];
static uint64_t deterministic_salt = 0;

void seed_deterministic_string(const std::string &string) {
    uint8_t digest[SHA256_DIGEST_LENGTH];
    sha256_Raw((uint8_t*)string.c_str(), string.length(), deterministic_seed);
    deterministic_salt = 0;
}

void deterministic_random(uint8_t* buf, size_t n) {
    deterministic_salt += 1;

    hkdf_sha256(buf, n,
    (uint8_t*)&deterministic_salt, sizeof(deterministic_salt),
    deterministic_seed, SHA256_DIGEST_LENGTH,
    NULL, 0);
}

std::vector<uint8_t> deterministic_random(std::vector<uint8_t> entropy, size_t n) {
    std::vector<uint8_t> result;
    result.resize(n);

    auto seed = sha256(entropy);

    hkdf_sha256(&result[0], n,
    NULL, 0, // no salt
    &seed[0], SHA256_DIGEST_LENGTH,
    NULL, 0); // no info

    return result;
}
