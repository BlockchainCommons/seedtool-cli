//
//  format-ints.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-ints.hpp"
#include "params.hpp"
#include "utils.hpp"

FormatInts::FormatInts() : Format(Format::Key::ints, "ints") {
    low = 0;
    high = 9;
}

void FormatInts::process_input(Params* p) {
}

void FormatInts::process_output(Params* p) {
    p->output = data_to_ints(p->seed, low, high, " ");
}
