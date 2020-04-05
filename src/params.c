//
//  params.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <memory.h>
#include <argp.h>

#include <bc-crypto-base/bc-crypto-base.h>

#include "params.h"
#include "utils.h"
#include "input.h"
#include "output.h"

params* new_params() {
    params* p = calloc(sizeof(params), 1);

    p->input_format = format_random;
    p->process_input = random_input_processor;

    p->output_format = format_hex;
    p->process_output = hex_output_processor;

    p->count = 32;

    //p->deterministic_seed = "test";

    if(p->deterministic_seed == NULL) {
        p->rng = crypto_random;
    } else {
        seed_deterministic_string(p->deterministic_seed);
        p->rng = deterministic_random;
    }

    // ints-specific
    p->low = 0;
    p->high = 9;

    // SLIP39-specific
    p->groups = calloc(1, sizeof(group_descriptor));
    p->groups[0] = (group_descriptor){1, 1, NULL};
    p->groups_count = 1;
    p->groups_threshold = 1;

    return p;
}

void dispose_params(params* p) {
    free(p->input);
    free(p->output);
    free(p->seed);
    free(p->groups);
    free(p);
}

char doc[] = "A program with a help and default options";
struct argp argp = { NULL, NULL, NULL, doc };
params* parse_params( int argc, char *argv[] ) {
    argp_parse( &argp, argc, argv, 0, 0, NULL );
    params* p = new_params();
    return p;
}
