//
//  format-bits.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bits.hpp"

#include <strings.h>

#include "params.hpp"
#include "utils.hpp"

void format_bits_process_output(format* f, params* p) {
    p->output = data_to_ints(p->seed, 0, 1, "");
}

static void format_bits_dispose(format* f) {
    free(f);
}

format* format_bits_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_bits;
    f->name = "bits";
    f->process_output = format_bits_process_output;
    f->dispose = format_bits_dispose;
    return f;
}
