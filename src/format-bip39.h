//
//  format-bip39.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef FORMAT_BIP39_H
#define FORMAT_BIP39_H

#include "format.h"

#include <stdbool.h>
#include <stdlib.h>

format* format_bip39_new();
bool format_bip39_is_seed_length_valid(size_t seed_len);

#endif /* FORMAT_BIP39_H */
