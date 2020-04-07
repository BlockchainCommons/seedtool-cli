//
//  format-base6.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-base6.hpp"
#include "params.hpp"
#include "utils.hpp"

void FormatBase6::process_input(Params* p) {
}

void FormatBase6::process_output(Params* p) {
    p->output = data_to_ints(p->seed, 0, 5, "");
}
