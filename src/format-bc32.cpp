//
//  format-bc32.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bc32.hpp"

#include <stdexcept>
#include <bc-bech32/bc-bech32.h>

#include "params.hpp"
#include "utils.hpp"

using namespace std;

bool FormatBC32::is_seed_length_valid(size_t seed_len) {
    if(!(1 <= seed_len && seed_len <= 64)) { return false; }
    return true;
}

void FormatBC32::process_input(Params* p) {
    auto input = p->get_one_argument();
    p->seed = bc32_to_data(input);
}

void FormatBC32::process_output(Params* p) {
    p->output = data_to_bc32(p->seed);
}
