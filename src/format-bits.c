//
//  format-bits.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bits.h"

#include <strings.h>

#include "params.h"
#include "utils.h"

void format_bits_process_output(format* this, params* p) {
    p->output = data_to_ints(p->seed, p->seed_len, 0, 1, "");
}

static void format_bits_dispose(format* this) {
    free(this);
}

format* format_bits_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_bits;
    f->name = "bits";
    f->process_output = format_bits_process_output;
    f->dispose = format_bits_dispose;
    return f;
}
