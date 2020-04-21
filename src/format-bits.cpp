//
//  format-bits.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bits.hpp"

#include <stdexcept>

#include "params.hpp"
#include "utils.hpp"
#include "random.hpp"

void FormatBits::process_input(Params* p) {
    auto input = p->get_one_argument();

    #if 0
    auto entropy = digits_to_data(input, 0, 1);
    p->seed = deterministic_random(entropy, p->count);
    #else
    // Compatibility with https://iancoleman.io/bip39/
    digits_to_data(input, 0, 1); // syntax check only
    p->seed = sha256_deterministic_random(input, p->count);
    #endif
}

void FormatBits::process_output(Params* p) {
    p->output = data_to_ints(p->seed, 0, 1, "");
}
