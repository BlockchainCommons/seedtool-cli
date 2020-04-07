//
//  format-base10.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-base10.h"
#include "params.h"
#include "utils.h"

void format_base10_process_output(format* this, params* p) {
    p->output = data_to_ints(p->seed, p->seed_len, 0, 9, "");
}

static void format_base10_dispose(format* this) {
    free(this);
}

format* format_base10_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_base10;
    f->name = "base10";
    f->process_output = format_base10_process_output;
    f->dispose = format_base10_dispose;
    return f;
}
