//
//  params.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <memory.h>
#include <argp.h>
#include <assert.h>

#include <bc-crypto-base/bc-crypto-base.h>

#include "params.h"
#include "utils.h"
#include "formats-all.h"

static raw_params* raw_params_new() {
    return calloc(sizeof(raw_params), 1);
}

static void raw_params_dispose(raw_params* this) {
    free(this);
}

static void raw_params_add_group(raw_params* this, const char* g) {
    if(this->slip39_groups_count == MAX_RAW_GROUPS) { return; }
    this->slip39_groups[this->slip39_groups_count] = g;
    this->slip39_groups_count++;
}

static void raw_params_add_arg(raw_params* this, const char* a) {
    if(this->args_count == MAX_ARGS) { return; }
    this->args[this->args_count] = a;
    this->args_count++;
}

static params* params_new() {
    params* p = calloc(sizeof(params), 1);
    p->raw = raw_params_new();

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
    raw_params_dispose(p->raw);
    format_dispose(p->input_format);
    format_dispose(p->output_format);
    free(p->input);
    free(p->output);
    free(p->seed);
    free(p);
}

static void params_validate_count(params* p, struct argp_state* state) {
    if(p->raw->count != NULL) {
        p->count = atoi(p->raw->count);
    } else {
        p->count = 32;
    }

    if(p->count < 1 || p->count > 64) {
        argp_error(state, "COUNT must be in [1-64].");
    }
}

static void params_validate_deterministic(params* p, struct argp_state* state) {
    if(p->raw->random_deterministic != NULL) {
        seed_deterministic_string(p->raw->random_deterministic);
        p->rng = deterministic_random;
    } else {
        p->rng = crypto_random;
    }
}

static void params_validate_input_format(params* p, struct argp_state* state) {
    raw_params* raw = p->raw;
    
    format* f = NULL;
    if(raw->input_format == NULL) {
        f = format_random_new();
    } else {
        format_key k = format_key_for_string(raw->input_format);
        switch(k) {
            case format_key_random: f = format_random_new(); break;
            case format_key_hex: f = format_hex_new(); break;
            case format_key_bits: f = format_bits_new(); break;
            case format_key_cards: f = format_cards_new(); break;
            case format_key_dice: f = format_dice_new(); break;
            case format_key_base6: f = format_base6_new(); break;
            case format_key_base10: f = format_base10_new(); break;
            case format_key_ints: f = format_ints_new(); break;
            case format_key_bip39: f = format_bip39_new(); break;
            case format_key_slip39: f = format_slip39_new(); break;
            default:
                argp_error(state, "Unknown input format: %s", raw->input_format);
                break;
        }
    }
    p->input_format = f;
}

static void params_validate_output_format(params* p, struct argp_state* state) {
    raw_params* raw = p->raw;
    
    format* f = NULL;
    if(raw->output_format == NULL) {
        f = format_hex_new();
    } else {
        format_key k = format_key_for_string(raw->output_format);
        switch(k) {
            case format_key_hex: f = format_hex_new(); break;
            case format_key_bits: f = format_bits_new(); break;
            case format_key_cards: f = format_cards_new(); break;
            case format_key_dice: f = format_dice_new(); break;
            case format_key_base6: f = format_base6_new(); break;
            case format_key_base10: f = format_base10_new(); break;
            case format_key_ints: f = format_ints_new(); break;
            case format_key_bip39: f = format_bip39_new(); break;
            case format_key_slip39: f = format_slip39_new(); break;
            default:
                argp_error(state, "Unknown output format: %s", raw->output_format);
                break;
        }
    }
    p->output_format = f;
}

static void params_validate_output_for_input(params* p, struct argp_state* state) {
    // Any input format works with hex output format.
    if(p->output_format->key == format_key_hex) {
        return;
    }

    // Random input works with any output format.
    if(p->input_format->key == format_key_random) {
        return;
    }

    // Hex input works with any output format.
    if(p->input_format->key == format_key_hex) {
        return;
    }

    argp_error(state, "Input format %s cannot be used with output format %s", 
        p->input_format->name, p->output_format->name);
}

static void params_validate_ints_specific(params* p, struct argp_state* state) {
    raw_params* raw = p->raw;

    format* f = p->output_format;
    if(f->key == format_key_ints) {
        int low = format_ints_get_low(f);
        int high = format_ints_get_high(f);
        if(raw->ints_low != NULL) {
            low = atoi(raw->ints_low);
        }
        if(raw->ints_high != NULL) {
            high = atoi(raw->ints_high);
        }
        if(!(0 <= low && low < high && high <= 255)) {
            argp_error(state, "--low and --high must specify a range in [0-255].");
        }
        format_ints_set_low(f, low);
        format_ints_set_high(f, high);
    } else {
        if(raw->ints_low != NULL) {
            argp_error(state, "Option --low can only be used with the \"ints\" output format.");
        }
        if(raw->ints_high != NULL) {
            argp_error(state, "Option --high can only be used with the \"ints\" output format.");
        }
    }
}

static void params_validate_bip39_specific(params* p, struct argp_state* state) {
    if(p->output_format->key != format_key_bip39) { return; }
    if(!format_bip39_is_seed_length_valid(p->count)) {
        argp_error(state, "For BIP39 COUNT must be in [12-32] and even.");
    }
}

void parse_group_spec(const char* string, group_descriptor* group, struct argp_state* state) {
    size_t threshold;
    size_t count;
    int items = sscanf(string, "%zd-of-%zd", &threshold, &count);
    if(items != 2) {
        argp_error(state, "Could not parse group specifier: \"%s\"", string);
    }
    if(!(0 < threshold && threshold <= count && count <= 16)) {
        argp_error(state, "Invalid group specifier \"%s\": 1 <= N <= M <= 16", string);
    }
    group->threshold = threshold;
    group->count = count;
    group->passwords = NULL;
}

static void params_validate_slip39_specific(params* p, struct argp_state* state) {
    raw_params* raw = p->raw;

    int groups_count = raw->slip39_groups_count;

    if(p->output_format->key != format_key_slip39) {
        if(groups_count > 0) {
            argp_error(state, "Option --group can only be used with the \"slip39\" output format.");
        }
        if(raw->slip39_groups_threshold != NULL) {
            argp_error(state, "Option --group-threshold can only be used with the \"slip39\" output format.");
        }
        return;
    }
    if(!format_slip39_is_seed_length_valid(p->count)) {
        argp_error(state, "For BIP39 COUNT must be in [16-32] and even.");
    }

    group_descriptor groups[MAX_GROUPS];
    if(groups_count > MAX_GROUPS) {
        argp_error(state, "There must be no more than %d groups.", MAX_GROUPS);
    } else if(groups_count == 0) {
        groups[0] = (group_descriptor){1, 1};
        groups_count = 1;     
    } else {
        for(int i = 0; i < groups_count; i++) {
            parse_group_spec(raw->slip39_groups[i], &groups[i], state);
        }
    }
    
    int groups_threshold;
    if(raw->slip39_groups_threshold == NULL) {
        groups_threshold = 1;
    } else {
        groups_threshold = atoi(raw->slip39_groups_threshold);
    }
    if(!(0 < groups_threshold && groups_threshold <= groups_count)) {
        argp_error(state, "Group threshold must be <= the number of groups.");
    }
    format_slip39_set_groups_threshold(p->output_format, groups_threshold);
    format_slip39_set_groups(p->output_format, groups, groups_count);
}

static void params_validate(params* p, struct argp_state* state) {
    params_validate_count(p, state);
    params_validate_deterministic(p, state);
    params_validate_input_format(p, state);
    params_validate_output_format(p, state);
    params_validate_output_for_input(p, state);
    params_validate_ints_specific(p, state);
    params_validate_bip39_specific(p, state);
    params_validate_slip39_specific(p, state);
}

static void parse_input_opt(params* p, const char* arg, struct argp_state* state) {
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    params* p = state->input;
    raw_params* raw = p->raw;

    switch (key) {
        case ARGP_KEY_INIT: break;
        case 'i': raw->input_format = arg; break;
        case 'o': raw->output_format = arg; break;
        case 'c': raw->count = arg; break;
        case 'l': raw->ints_low = arg; break;
        case 'h': raw->ints_high = arg; break;
        case 'd': raw->random_deterministic = arg; break;
        case 'g': raw_params_add_group(raw, arg); break;
        case 't': raw->slip39_groups_threshold = arg; break;
        case ARGP_KEY_ARG: raw_params_add_arg(raw, arg); break;
        case ARGP_KEY_END: {
            params_validate(p, state);
        }
        break;
    }
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
    {"group-threshold", 't', "1-16", 0, "The number of groups that must contribute (default: 1)"},
    {"group", 'g', "M-of-N", 0, "The group specification (default: 1-of-1)"},
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
