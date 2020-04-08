//
//  format-hex.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-hex.hpp"

#include <iostream>

#include "params.hpp"
#include "utils.hpp"

void FormatHex::process_input(Params* p) {
    auto input = p->get_one_argument();
    p->seed = hex_to_data(input);
}

void FormatHex::process_output(Params* p) {
    p->output = data_to_hex(p->seed);
}
