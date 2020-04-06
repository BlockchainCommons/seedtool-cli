//
//  format-hex.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-hex.h"
#include "params.h"
#include "utils.h"

void format_hex_process_output(format* this, params* p) {
    p->output = data_to_hex(p->seed, p->seed_len);
}

static void format_hex_dispose(format* this) {
    free(this);
}

format* format_hex_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_hex;
    f->name = "hex";
    f->process_output = format_hex_process_output;
    f->dispose = format_hex_dispose;
    return f;
}
