//
//  format-random.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-random.h"
#include "random.h"
#include "params.h"

typedef struct format_random_options_t {
    char* deterministic_seed;
    random_generator rng;
} format_random_options;

static void format_random_process_input(format* this, params* p) {
    p->seed_len = p->count;
    p->seed = malloc(p->seed_len);
    format_random_options* opts = this->format_options;
    opts->rng(p->seed, p->seed_len);
}

static void format_random_set_defaults(format* this) {
    format_random_options* o = this->format_options;
    if(o->deterministic_seed == NULL) {
        o->rng = crypto_random;
    } else {
        seed_deterministic_string(o->deterministic_seed);
        o->rng = deterministic_random;
    }
}

static void format_random_dispose(format* this) {
    free(this->format_options);
    free(this);
}

format* format_random_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_random;
    f->name = "random";
    f->format_options = calloc(sizeof(format_random_options), 1);
    f->set_defaults = format_random_set_defaults;
    f->process_input = format_random_process_input;
    f->dispose = format_random_dispose;
    return f;
}
