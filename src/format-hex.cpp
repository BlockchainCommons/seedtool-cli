//
//  format-hex.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-hex.hpp"
#include "params.hpp"
#include "utils.hpp"

void format_hex_process_output(format* f, params* p) {
    p->output = data_to_hex(p->seed);
}

static void format_hex_dispose(format* f) {
    free(f);
}

format* format_hex_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_hex;
    f->name = "hex";
    f->process_output = format_hex_process_output;
    f->dispose = format_hex_dispose;
    return f;
}
