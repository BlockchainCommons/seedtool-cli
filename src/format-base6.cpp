//
//  format-base6.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-base6.hpp"
#include "params.hpp"
#include "utils.hpp"

void format_base6_process_output(format* f, params* p) {
    p->output = data_to_ints(p->seed, 0, 5, "");
}

static void format_base6_dispose(format* f) {
    free(f);
}

format* format_base6_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_base6;
    f->name = "base6";
    f->process_output = format_base6_process_output;
    f->dispose = format_base6_dispose;
    return f;
}
