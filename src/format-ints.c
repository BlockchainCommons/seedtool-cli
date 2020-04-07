//
//  format-ints.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-ints.h"
#include "params.h"
#include "utils.h"

typedef struct format_ints_options_t {
    size_t low;
    size_t high;
} format_ints_options;

void format_ints_process_output(format* this, params* p) {
    format_ints_options* opts = this->format_options;
    p->output = data_to_ints(p->seed, p->seed_len, opts->low, opts->high, " ");
}

static void format_ints_dispose(format* this) {
    free(this->format_options);
    free(this);
}

format* format_ints_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_ints;
    f->name = "ints";
    format_ints_options* opts = calloc(sizeof(format_ints_options), 1);
    opts->low = 0;
    opts->high = 9;
    f->format_options = opts;
    f->process_output = format_ints_process_output;
    f->dispose = format_ints_dispose;
    return f;
}

size_t format_ints_get_low(const format* f) {
    format_ints_options* opts = f->format_options;
    return opts->low;
}

size_t format_ints_get_high(const format* f) {
    format_ints_options* opts = f->format_options;
    return opts->high;
}

void format_ints_set_low(format* f, size_t low) {
    format_ints_options* opts = f->format_options;
    opts->low = low;
}

void format_ints_set_high(format* f, size_t high) {
    format_ints_options* opts = f->format_options;
    opts->high = high;
}
