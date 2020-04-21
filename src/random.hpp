//
//  random.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>

typedef void (*random_generator)(uint8_t*, size_t);

// Generates a buffer of random bytes using the OS's cryptographically strong random number generator.
void crypto_random(uint8_t* buf, size_t n);

// Seeds the cryptographically strong deterministic random number generator with the SHA256 digest of the string.
void seed_deterministic_string(const std::string &string);

// Generates a buffer of random bytes using the cryptographically strong deterministic random number generator.
void deterministic_random(uint8_t* buf, size_t n);

std::vector<uint8_t> deterministic_random(const std::vector<uint8_t> &entropy, size_t n);

std::vector<uint8_t> sha256_deterministic_random(const std::vector<uint8_t> &entropy, size_t n);
std::vector<uint8_t> sha256_deterministic_random(const std::string &string, size_t n);
