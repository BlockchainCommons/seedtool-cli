//
//  format-random.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-random.h"
#include "params.h"

static void format_random_process_input(format* this, params* p) {
    p->seed_len = p->count;
    p->seed = malloc(p->seed_len);
    p->rng(p->seed, p->seed_len);
}

static void format_random_dispose(format* this) {
    free(this);
}

format* format_random_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_random;
    f->name = "random";
    f->process_input = format_random_process_input;
    f->dispose = format_random_dispose;
    return f;
}
