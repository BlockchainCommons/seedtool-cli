//
//  format-dice.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-dice.h"
#include "params.h"
#include "utils.h"

void format_dice_process_output(format* f, params* p) {
    p->output = data_to_ints(p->seed, p->seed_len, 1, 6, "");
}

static void format_dice_dispose(format* f) {
    free(f);
}

format* format_dice_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_dice;
    f->name = "dice";
    f->process_output = format_dice_process_output;
    f->dispose = format_dice_dispose;
    return f;
}
