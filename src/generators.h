//
//  generators.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef GENERATORS_H
#define GENERATORS_H

#include <stdlib.h>
#include <bc-slip39/bc-slip39.h>

char* random_hex(size_t count);
char* random_binary(size_t count);
char* random_cards(size_t count);
char* random_ints(size_t low, size_t high, size_t count, const char* separator);
char* random_bip39(size_t count);
char* random_slip39(size_t secret_len, size_t group_threshold, const group_descriptor* groups, size_t group_count);

#endif /* GENERATORS_H */