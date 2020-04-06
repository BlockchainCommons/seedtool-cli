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
#include "random.h"

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

static int parse_opt(int key, char* arg, struct argp_state* state) {
    int* arg_count = state->input;
    switch (key) {
        case 'i': {
            printf("in: %s\n", arg);
        }
        break;
        case 'o': {
            printf("out: %s\n", arg);
        }
        break;
        case 'c': {
            printf("count: %s\n", arg);
        }
        break;
        case 1000: {
            printf("low: %s\n", arg);
        }
        break;
        case 1001: {
            printf("high: %s\n", arg);
        }
        break;
        case ARGP_KEY_INIT: {
            printf("INIT\n");
        }
        break;
        case ARGP_KEY_ARG: {
            printf("arg: %s\n", arg);
        }
        break;
        case ARGP_KEY_END: {
            printf("END\n");
        }
        break;
    }
    return 0;
}

struct argp_option options[] = {
    {"in", 'i', "[random|hex|bits|cards|dice|base6|base10|ints|bip39|slip39]", 0, "Specify the input format (default: random)"},
    {"out", 'o', "[hex|bits|cards|dice|base6|base10|ints|bip39|slip39]", 0, "Specify the output format (default: hex)."},
    {"count", 'c', "N", 0, "Specify the count of output units (default: 32)"},
    {"deterministic", 'd', "SEED", 0, "Use a deterministic random number generator with SEED"},
    {0, 0, 0, 0, "ints Input and Output Options:", 7},
    {"low", 1000, "LOW", 0, "The lowest int returned (default: 1)"},
    {"high", 1001, "HIGH", 0, "The highest int returned (default: 9)"},
    {0, 0, 0, 0, "SLIP39 Output Options:", 6},
    {"group-threshold", 1002, "N", 0, "The number of groups that must contribute (default: 1)"},
    {"group", 1003, "N-of-M", 0, "The group specification (default: 1-of-1)"},
    { 0 }
};

const char* argp_program_version = "version 1.0";
const char* argp_program_bug_address = "who@dunnit.com";

char doc[] = "Converts cryptographic seeds between various forms.";
struct argp argp = { options, parse_opt, "WORD [WORD [WORD [WORD]]]", doc };

params* parse_params( int argc, char *argv[] ) {
    params* p = new_params();
    argp_parse( &argp, argc, argv, 0, 0, p );
    return p;
}
