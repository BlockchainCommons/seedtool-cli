//
//  format-random.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-random.hpp"
#include "params.hpp"

void FormatRandom::process_input(Params* p) {
    p->seed.resize(p->count);
    p->rng(p->seed.data(), p->count);
}

void FormatRandom::process_output(Params* p) {
    assert(false); // never happens
}
