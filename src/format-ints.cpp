//
//  format-ints.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-ints.hpp"
#include "params.hpp"
#include "utils.hpp"

struct format_ints_options {
    size_t low;
    size_t high;
};

void format_ints_process_output(format* f, params* p) {
    format_ints_options* opts = (format_ints_options*)f->format_options;
    p->output = data_to_ints(p->seed, opts->low, opts->high, " ");
}

static void format_ints_dispose(format* f) {
    free(f->format_options);
    free(f);
}

format* format_ints_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_ints;
    f->name = "ints";
    format_ints_options* opts = (format_ints_options*)calloc(sizeof(format_ints_options), 1);
    opts->low = 0;
    opts->high = 9;
    f->format_options = opts;
    f->process_output = format_ints_process_output;
    f->dispose = format_ints_dispose;
    return f;
}

size_t format_ints_get_low(const format* f) {
    format_ints_options* opts = (format_ints_options*)f->format_options;
    return opts->low;
}

size_t format_ints_get_high(const format* f) {
    format_ints_options* opts = (format_ints_options*)f->format_options;
    return opts->high;
}

void format_ints_set_low(format* f, size_t low) {
    format_ints_options* opts = (format_ints_options*)f->format_options;
    opts->low = low;
}

void format_ints_set_high(format* f, size_t high) {
    format_ints_options* opts = (format_ints_options*)f->format_options;
    opts->high = high;
}
