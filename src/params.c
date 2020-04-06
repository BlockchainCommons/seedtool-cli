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
#include "random.h"
#include "formats-all.h"

static params* params_new() {
    params* p = calloc(sizeof(params), 1);

    // p->input_format = format_random;
    // p->output_format = hex_format;
    // p->count = 32;

    //p->deterministic_seed = "test";

    // if(p->deterministic_seed == NULL) {
    //     p->rng = crypto_random;
    // } else {
    //     seed_deterministic_string(p->deterministic_seed);
    //     p->rng = deterministic_random;
    // }

    // // ints-specific
    // p->low = 0;
    // p->high = 9;

    // // SLIP39-specific
    // p->groups = calloc(1, sizeof(group_descriptor));
    // p->groups[0] = (group_descriptor){1, 1, NULL};
    // p->groups_count = 1;
    // p->groups_threshold = 1;

    return p;
}

void params_dispose(params* p) {
    if (p == NULL) { return; }
    format_dispose(p->input_format);
    format_dispose(p->output_format);
    free(p->input);
    free(p->output);
    free(p->seed);
    free(p);
}

static void params_set_defaults(params* p) {
    if (p->input_format == NULL) {
        p->input_format = format_random_new();
    }

    if (p->output_format == NULL) {
        p->output_format = format_hex_new();
    }

    if (p->count == 0) {
        p->count = 32;
    }

    if(p->input_format->set_defaults != NULL) {
        p->input_format->set_defaults(p->input_format);
    }
}

static void params_validate(params* p) {
}

static void parse_input_opt(params* p, const char* arg, struct argp_state* state) {
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    params* p = state->input;

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
        case 'l': {
            printf("low: %s\n", arg);
        }
        break;
        case 'h': {
            printf("high: %s\n", arg);
        }
        break;
        case 'd': {
            printf("deterministic: %s\n", arg);
        }
        break;
        case 'g': {
            printf("group: %s\n", arg);
        }
        break;
        case 't': {
            printf("group-threshold: %s\n", arg);
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
    params_set_defaults(p);
    params_validate(p);
    return 0;
}

struct argp_option options[] = {
    {"in", 'i', "random|hex|bits|cards|dice|base6|base10|ints|bip39|slip39", 0, "The input format (default: random)"},
    {"out", 'o', "hex|bits|cards|dice|base6|base10|ints|bip39|slip39", 0, "The output format (default: hex)"},
    {"count", 'c', "1-64", 0, "The number of output units (default: 32)"},

    {0, 0, 0, 0, "ints Input and Output Options:", 1},
    {"low", 'l', "0-254", 0, "The lowest int returned (default: 1)"},
    {"high", 'h', "1-255", 0, "The highest int returned (default: 9)"},
    {"low < high", 0, 0, OPTION_DOC, 0},

    {0, 0, 0, 0, "SLIP39 Output Options:", 2},
    {"group-threshold", 'g', "1-16", 0, "The number of groups that must contribute (default: 1)"},
    {"group", 't', "M-of-N", 0, "The group specification (default: 1-of-1)"},
    {"The --group option may appear more than once.", 0, 0, OPTION_DOC, 0},
    {"M < N", 0, 0, OPTION_DOC, 0},
    {"The group threshold must be <= the number of group specifications.", 0, 0, OPTION_DOC, 0},

    {0, 0, 0, 0, "Deterministic Random Numbers:", 3},
    {"deterministic", 'd', "SEED", 0, "Use a deterministic random number generator with SEED"},

    { 0 }
};

const char* argp_program_version = "1.0";
// const char* argp_program_bug_address = "who@dunnit.com";

char doc[] = "Converts cryptographic seeds between various forms.";
struct argp argp = { options, parse_opt, "INPUT", doc };

params* params_parse( int argc, char *argv[] ) {
    params* p = params_new();
    argp_parse( &argp, argc, argv, 0, 0, p );
    return p;
}
