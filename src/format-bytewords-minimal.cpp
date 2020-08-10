//
//  format-bytewords.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bytewords-minimal.hpp"

#include <stdexcept>
#include <bc-ur/bc-ur.hpp>

#include "params.hpp"
#include "utils.hpp"

using namespace std;

bool FormatBytewordsMinimal::is_seed_length_valid(size_t seed_len) {
    if(!(1 <= seed_len && seed_len <= 64)) { return false; }
    return true;
}

void FormatBytewordsMinimal::process_input(Params* p) {
    auto input = p->get_one_argument();
    p->seed = ur::Bytewords::decode(ur::Bytewords::style::minimal, input);
}

void FormatBytewordsMinimal::process_output(Params* p) {
    p->output = ur::Bytewords::encode(ur::Bytewords::style::minimal, p->seed);
}
