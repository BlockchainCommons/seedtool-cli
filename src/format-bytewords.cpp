//
//  format-bytewords.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bytewords.hpp"

#include <stdexcept>
#include <bc-ur/bc-ur.hpp>

#include "params.hpp"
#include "utils.hpp"

using namespace std;

bool FormatBytewords::is_seed_length_valid(size_t seed_len) {
    if(!(1 <= seed_len && seed_len <= 64)) { return false; }
    return true;
}

void FormatBytewords::process_input(Params* p) {
    auto input = p->get_one_argument();
    p->seed = ur::Bytewords::decode(ur::Bytewords::style::standard, input);
}

void FormatBytewords::process_output(Params* p) {
    p->output = ur::Bytewords::encode(ur::Bytewords::style::standard, p->seed);
}
