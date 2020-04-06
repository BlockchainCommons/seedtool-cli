//
//  random.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>
#include <stdlib.h>

// Generates a buffer of random bytes using the OS's cryptographically strong random number generator.
void crypto_random(uint8_t* buf, size_t n);

// Seeds the cryptographically strong deterministic random number generator with the SHA256 digest of the string.
void seed_deterministic_string(const char* string);

// Generates a buffer of random bytes using the cryptographically strong deterministic random number generator.
void deterministic_random(uint8_t* buf, size_t n);

#endif /* RANDOM_H */
