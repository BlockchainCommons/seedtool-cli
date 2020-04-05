//
//  output.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "output.h"
#include "utils.h"

void hex_output_processor(params* p) {
    p->output = data_to_hex(p->seed, p->seed_len);
}
