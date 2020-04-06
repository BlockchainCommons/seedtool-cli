//
//  format.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format.h"
#include <stdlib.h>
#include "utils.h"

void format_dispose(format* f) {
    if(f == NULL) { return; }
    if(f->dispose != NULL) {
        f->dispose(f);
    }
}

static const char* format_key_names[] = {
    "unknown",
    "random",
    "hex",
    "bits",
    "cards",
    "dice",
    "base6",
    "base10",
    "ints",
    "bip39",
    "slip39"
};

static const size_t format_key_count = 10;

static const char* string_for_format_key(format_key key) {
    if(key < 0) { return NULL; }
    if(key >= format_key_count) { return NULL; }

    return format_key_names[key];
}

static format_key format_key_for_string(const char* arg) {
    if(arg == NULL) { return format_key_unknown; }

    for(int i = 0; i < format_key_count; i++) {
        if(equal_strings(arg, format_key_names[i])) {
            return i;
        }
    }

    return format_key_unknown;
}
