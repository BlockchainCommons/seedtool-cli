//
//  format-base6.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-base6.h"
#include "params.h"
#include "utils.h"

void format_base6_process_output(format* this, params* p) {
    p->output = data_to_ints(p->seed, p->seed_len, 0, 5, "");
}

static void format_base6_dispose(format* this) {
    free(this);
}

format* format_base6_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_base6;
    f->name = "base6";
    f->process_output = format_base6_process_output;
    f->dispose = format_base6_dispose;
    return f;
}
