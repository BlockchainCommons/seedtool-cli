//
//  params.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <memory.h>
#include <argp.h>
#include <assert.h>

#include <bc-crypto-base/bc-crypto-base.h>

#include "params.hpp"
#include "utils.hpp"
#include "formats-all.hpp"

void raw_params::add_group(const char* g) {
    if(slip39_groups_count == MAX_RAW_GROUPS) { return; }
    slip39_groups[slip39_groups_count] = g;
    slip39_groups_count++;
}

void raw_params::add_arg(const char* a) {
    if(args_count == MAX_ARGS) { return; }
    args[args_count] = a;
    args_count++;
}

params::~params() {
    delete raw;
    format_dispose(input_format);
    format_dispose(output_format);
    free(input);
}

void params::validate_count(struct argp_state* state) {
    if(raw->count != NULL) {
        count = atoi(raw->count);
    } else {
        count = 32;
    }

    if(count < 1 || count > 64) {
        argp_error(state, "COUNT must be in [1-64].");
    }
}

void params::validate_deterministic(struct argp_state* state) {
    if(raw->random_deterministic != NULL) {
        seed_deterministic_string(raw->random_deterministic);
        rng = deterministic_random;
    } else {
        rng = crypto_random;
    }
}

void params::validate_input_format(struct argp_state* state) {    
    format* f = NULL;
    if(raw->input_format == NULL) {
        f = format_random_new();
    } else {
        auto k = format_key_for_string(raw->input_format);
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
    input_format = f;
}

void params::validate_output_format(struct argp_state* state) {    
    format* f = NULL;
    if(raw->output_format == NULL) {
        f = format_hex_new();
    } else {
        auto k = format_key_for_string(raw->output_format);
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
    output_format = f;
}

void params::validate_output_for_input(struct argp_state* state) {
    // Any input format works with hex output format.
    if(output_format->key == format_key_hex) {
        return;
    }

    // Random input works with any output format.
    if(input_format->key == format_key_random) {
        return;
    }

    // Hex input works with any output format.
    if(input_format->key == format_key_hex) {
        return;
    }

    argp_error(state, "Input format %s cannot be used with output format %s", 
        input_format->name, output_format->name);
}

void params::validate_ints_specific(struct argp_state* state) {
    format* f = output_format;
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

void params::validate_bip39_specific(struct argp_state* state) {
    if(output_format->key != format_key_bip39) { return; }
    if(!format_bip39_is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [12-32] and even.");
    }
}

void parse_group_spec(const char* string, group_descriptor* group, struct argp_state* state) {
    size_t threshold;
    size_t count;
    auto items = sscanf(string, "%zd-of-%zd", &threshold, &count);
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

void params::validate_slip39_specific(struct argp_state* state) {
    auto groups_count = raw->slip39_groups_count;

    if(output_format->key != format_key_slip39) {
        if(groups_count > 0) {
            argp_error(state, "Option --group can only be used with the \"slip39\" output format.");
        }
        if(raw->slip39_groups_threshold != NULL) {
            argp_error(state, "Option --group-threshold can only be used with the \"slip39\" output format.");
        }
        return;
    }
    if(!format_slip39_is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [16-32] and even.");
    }

    group_descriptor groups[MAX_GROUPS];
    if(groups_count > MAX_GROUPS) {
        argp_error(state, "There must be no more than %d groups.", MAX_GROUPS);
    } else if(groups_count == 0) {
        groups[0] = (group_descriptor){1, 1};
        groups_count = 1;     
    } else {
        for(auto i = 0; i < groups_count; i++) {
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
    format_slip39_set_groups_threshold(output_format, groups_threshold);
    format_slip39_set_groups(output_format, groups, groups_count);
}

void params::validate(struct argp_state* state) {
    validate_count(state);
    validate_deterministic(state);
    validate_input_format(state);
    validate_output_format(state);
    validate_output_for_input(state);
    validate_ints_specific(state);
    validate_bip39_specific(state);
    validate_slip39_specific(state);
}

static void parse_input_opt(params* p, const char* arg, struct argp_state* state) {
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    auto p = static_cast<params*>(state->input);
    auto raw = p->raw;

    switch (key) {
        case ARGP_KEY_INIT: break;
        case 'i': raw->input_format = arg; break;
        case 'o': raw->output_format = arg; break;
        case 'c': raw->count = arg; break;
        case 'l': raw->ints_low = arg; break;
        case 'h': raw->ints_high = arg; break;
        case 'd': raw->random_deterministic = arg; break;
        case 'g': raw->add_group(arg); break;
        case 't': raw->slip39_groups_threshold = arg; break;
        case ARGP_KEY_ARG: raw->add_arg(arg); break;
        case ARGP_KEY_END: {
            p->validate(state);
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

auto argp_program_version = "1.0";
// const char* argp_program_bug_address = "who@dunnit.com";

auto doc = "Converts cryptographic seeds between various forms.";
struct argp argp = { options, parse_opt, "INPUT", doc };

params* params_parse( int argc, char *argv[] ) {
    auto p = new params();
    argp_parse( &argp, argc, argv, 0, 0, p );
    return p;
}
