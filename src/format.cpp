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

static const std::vector<std::string> format_key_names = {
    "random",
    "hex",
    "bits",
    "cards",
    "dice",
    "base6",
    "base10",
    "ints",
    "bip39",
    "sskr",
    "btw",
    "btwu",
    "btwm"
};

Format::Key Format::key_for_string(const std::string &arg) {
    for(auto i = 0; i < format_key_names.size(); i++) {
        if(arg == format_key_names[i]) {
            return (Format::Key)i;
        }
    }

    return Format::Key::unknown;
}
