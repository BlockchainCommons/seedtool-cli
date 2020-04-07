//
//  format-bits.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bits.hpp"
#include "params.hpp"
#include "utils.hpp"

void FormatBits::process_input(Params* p) {
}

void FormatBits::process_output(Params* p) {
    p->output = data_to_ints(p->seed, 0, 1, "");
}
