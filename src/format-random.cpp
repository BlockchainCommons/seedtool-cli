//
//  format-random.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-random.hpp"
#include "params.hpp"

static void format_random_process_input(format* f, params* p) {
    p->seed.resize(p->count);
    p->rng(&p->seed[0], p->count);
}

static void format_random_dispose(format* f) {
    free(f);
}

format* format_random_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_random;
    f->name = "random";
    f->process_input = format_random_process_input;
    f->dispose = format_random_dispose;
    return f;
}
