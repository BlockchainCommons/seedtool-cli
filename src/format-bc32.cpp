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

using namespace std;

bool FormatBC32::is_seed_length_valid(size_t seed_len) {
    if(!(1 <= seed_len && seed_len <= 64)) { return false; }
    return true;
}

void FormatBC32::process_input(Params* p) {
    auto input = p->get_one_argument();
    vector<uint8_t> seed;
    seed.resize(300);
    size_t seed_len = 0;
    if(bc32_seed_decode(&seed[0], &seed_len, input.c_str()) == 0) {
        throw runtime_error("Invalid BC32 seed.");
    }
    seed.resize(seed_len);
    p->seed = seed;
}

void FormatBC32::process_output(Params* p) {
    auto data_len = p->seed.size();
    char output[200];
    auto data = &p->seed[0];
    if(bc32_seed_encode(output, data, data_len) == 0) {
        throw runtime_error("BC32 seed encoding failed.");
    }
    p->output = output;
}
