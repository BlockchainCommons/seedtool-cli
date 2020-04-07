//
//  format-base10.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-base10.hpp"
#include "params.hpp"
#include "utils.hpp"

void format_base10_process_output(format* f, params* p) {
    p->output = data_to_ints(p->seed, 0, 9, "");
}

static void format_base10_dispose(format* f) {
    free(f);
}

format* format_base10_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_base10;
    f->name = "base10";
    f->process_output = format_base10_process_output;
    f->dispose = format_base10_dispose;
    return f;
}
