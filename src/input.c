//
//  input.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "input.h"
#include <memory.h>

void random_input_processor(params* p) {
    p->seed_len = p->count;
    p->seed = malloc(p->seed_len);
    p->rng(p->seed, p->seed_len);
}
