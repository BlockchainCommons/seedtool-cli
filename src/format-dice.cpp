//
//  format-dice.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-dice.hpp"
#include "params.hpp"
#include "utils.hpp"

void FormatDice::process_input(Params* p) {
}

void FormatDice::process_output(Params* p) {
    p->output = data_to_ints(p->seed, 1, 6, "");
}
