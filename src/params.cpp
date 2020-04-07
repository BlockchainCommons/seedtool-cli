//
//  params.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <memory.h>
#include <argp.h>
#include <assert.h>
#include <iostream>

#include <bc-crypto-base/bc-crypto-base.h>

#include "params.hpp"
#include "utils.hpp"
#include "formats-all.hpp"

Params::~Params() {
    delete input_format;
    delete output_format;
}

void Params::validate_count(struct argp_state* state) {
    if(raw.count != "") {
        count = std::stoi(raw.count);
    } else {
        count = 32;
    }

    if(count < 1 || count > 64) {
        argp_error(state, "COUNT must be in [1-64].");
    }
}

void Params::validate_deterministic(struct argp_state* state) {
    if(raw.random_deterministic != "") {
        seed_deterministic_string(raw.random_deterministic);
        rng = deterministic_random;
    } else {
        rng = crypto_random;
    }
}

void Params::validate_input_format(struct argp_state* state) {    
    if(raw.input_format == "") {
        input_format = new FormatRandom();
    } else {
        auto k = Format::key_for_string(raw.input_format);
        switch(k) {
            case Format::Key::random: input_format = new FormatRandom(); break;
            case Format::Key::hex: input_format = new FormatHex(); break;
            case Format::Key::bits: input_format = new FormatBits(); break;
            case Format::Key::cards: input_format = new FormatCards(); break;
            case Format::Key::dice: input_format = new FormatDice(); break;
            case Format::Key::base6: input_format = new FormatBase6(); break;
            case Format::Key::base10: input_format = new FormatBase10(); break;
            case Format::Key::ints: input_format = new FormatInts(); break;
            case Format::Key::bip39: input_format = new FormatBIP39(); break;
            case Format::Key::slip39: input_format = new FormatSLIP39(); break;
            default:
                argp_error(state, "Unknown input format: %s", raw.input_format.c_str());
                break;
        }
    }
}

void Params::validate_output_format(struct argp_state* state) {    
    if(raw.output_format == "") {
        output_format = new FormatHex();
    } else {
        auto k = Format::key_for_string(raw.output_format);
        switch(k) {
            case Format::Key::hex: output_format = new FormatHex(); break;
            case Format::Key::bits: output_format = new FormatBits(); break;
            case Format::Key::cards: output_format = new FormatCards(); break;
            case Format::Key::dice: output_format = new FormatDice(); break;
            case Format::Key::base6: output_format = new FormatBase6(); break;
            case Format::Key::base10: output_format = new FormatBase10(); break;
            case Format::Key::ints: output_format = new FormatInts(); break;
            case Format::Key::bip39: output_format = new FormatBIP39(); break;
            case Format::Key::slip39: output_format = new FormatSLIP39(); break;
            default:
                argp_error(state, "Unknown output format: %s", raw.output_format.c_str());
                break;
        }
    }
}

void Params::validate_output_for_input(struct argp_state* state) {
    // Any input format works with hex output format.
    if(output_format->key == Format::Key::hex) {
        return;
    }

    // Random input works with any output format.
    if(input_format->key == Format::Key::random) {
        return;
    }

    // Hex input works with any output format.
    if(input_format->key == Format::Key::hex) {
        return;
    }

    argp_error(state, "Input format %s cannot be used with output format %s", 
        input_format->name.c_str(), output_format->name.c_str());
}

void Params::validate_ints_specific(struct argp_state* state) {
    Format* f = output_format;
    if(f->key == Format::Key::ints) {
        auto fi = dynamic_cast<FormatInts*>(f);
        int low = fi->low;
        int high = fi->low;
        if(raw.ints_low != "") {
            low = std::stoi(raw.ints_low);
        }
        if(raw.ints_high != "") {
            high = std::stoi(raw.ints_high);
        }
        if(!(0 <= low && low < high && high <= 255)) {
            argp_error(state, "--low and --high must specify a range in [0-255].");
        }
        fi->low = low;
        fi->high = high;
    } else {
        if(raw.ints_low != "") {
            argp_error(state, "Option --low can only be used with the \"ints\" output format.");
        }
        if(raw.ints_high != "") {
            argp_error(state, "Option --high can only be used with the \"ints\" output format.");
        }
    }
}

void Params::validate_bip39_specific(struct argp_state* state) {
    if(output_format->key != Format::Key::bip39) { return; }
    if(!FormatBIP39::is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [12-32] and even.");
    }
}

void parse_group_spec(const std::string &string, group_descriptor* group, struct argp_state* state) {
    size_t threshold;
    size_t count;
    auto items = sscanf(string.c_str(), "%zd-of-%zd", &threshold, &count);
    if(items != 2) {
        argp_error(state, "Could not parse group specifier: \"%s\"", string.c_str());
    }
    if(!(0 < threshold && threshold <= count && count <= 16)) {
        argp_error(state, "Invalid group specifier \"%s\": 1 <= N <= M <= 16", string.c_str());
    }
    group->threshold = threshold;
    group->count = count;
    group->passwords = NULL;
}

void Params::validate_slip39_specific(struct argp_state* state) {
    auto groups_count = raw.slip39_groups.size();

    if(output_format->key != Format::Key::slip39) {
        if(groups_count > 0) {
            argp_error(state, "Option --group can only be used with the \"slip39\" output format.");
        }
        if(raw.slip39_groups_threshold != "") {
            argp_error(state, "Option --group-threshold can only be used with the \"slip39\" output format.");
        }
        return;
    }
    if(!FormatSLIP39::is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [16-32] and even.");
    }

    std::vector<group_descriptor> groups;
    if(groups_count > MAX_GROUPS) {
        argp_error(state, "There must be no more than %d groups.", MAX_GROUPS);
    } else if(groups_count == 0) {
        groups.push_back( (group_descriptor){1, 1} );
    } else {
        for(auto i = 0; i < groups_count; i++) {
            parse_group_spec(raw.slip39_groups[i], &groups[i], state);
        }
    }
    
    int groups_threshold;
    if(raw.slip39_groups_threshold == "") {
        groups_threshold = 1;
    } else {
        groups_threshold = std::stoi(raw.slip39_groups_threshold);
    }
    if(!(0 < groups_threshold && groups_threshold <= groups_count)) {
        argp_error(state, "Group threshold must be <= the number of groups.");
    }
    auto of = dynamic_cast<FormatSLIP39*>(output_format);
    of->groups_threshold = groups_threshold;
    of->groups = groups;
}

void Params::validate(struct argp_state* state) {
    validate_count(state);
    validate_deterministic(state);
    validate_input_format(state);
    validate_output_format(state);
    validate_output_for_input(state);
    validate_ints_specific(state);
    validate_bip39_specific(state);
    validate_slip39_specific(state);
}

static void parse_input_opt(Params* p, const char* arg, struct argp_state* state) {
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    auto p = static_cast<Params*>(state->input);
    auto raw = p->raw;

    switch (key) {
        case ARGP_KEY_INIT: break;
        case 'i': raw.input_format = arg; break;
        case 'o': raw.output_format = arg; break;
        case 'c': raw.count = arg; break;
        case 'l': raw.ints_low = arg; break;
        case 'h': raw.ints_high = arg; break;
        case 'd': raw.random_deterministic = arg; break;
        case 'g': raw.slip39_groups.push_back(arg); break;
        case 't': raw.slip39_groups_threshold = arg; break;
        case ARGP_KEY_ARG: raw.args.push_back(arg); break;
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

Params* Params::parse( int argc, char *argv[] ) {
    auto p = new Params();
    argp_parse( &argp, argc, argv, 0, 0, p );
    return p;
}
