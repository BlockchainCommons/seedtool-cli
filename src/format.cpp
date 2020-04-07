//
//  format.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format.hpp"

#include <stdlib.h>
#include <string>
#include <stdexcept>

#include "utils.hpp"

void format_dispose(format* f) {
    if(f == NULL) { return; }
    if(f->dispose != NULL) {
        f->dispose(f);
    }
}

static const std::string format_key_names[] = {
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

static const std::string& string_for_format_key(format_key key) {
    if(key < 0 || key > format_key_count) {
        throw std::runtime_error("Unknown format key.");
    }

    return format_key_names[key];
}

format_key format_key_for_string(const char* arg) {
    if(arg == NULL) { return format_key_unknown; }

    for(int i = 0; i < format_key_count; i++) {
        if(std::string(arg) == format_key_names[i]) {
            return (format_key)i;
        }
    }

    return format_key_unknown;
}
