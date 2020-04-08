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
#include <stdexcept>

#include <bc-crypto-base/bc-crypto-base.h>

#include "params.hpp"
#include "utils.hpp"
#include "formats-all.hpp"
#include "config.h"

using namespace std;

Params::~Params() {
    delete input_format;
    delete output_format;
}

void Params::validate_count() {
    if(!raw.count.empty()) {
        count = stoi(raw.count);
    } else {
        count = 16;
    }

    if(count < 1 || count > 64) {
        argp_error(state, "COUNT must be in [1-64].");
    }
}

void Params::validate_deterministic() {
    if(!raw.random_deterministic.empty()) {
        seed_deterministic_string(raw.random_deterministic);
        rng = deterministic_random;
    } else {
        rng = crypto_random;
    }
}

void Params::validate_input_format() {    
    if(raw.input_format.empty()) {
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

void Params::validate_output_format() {    
    if(raw.output_format.empty()) {
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

void Params::validate_output_for_input() {
    // Any input format works with hex output format.
    if(dynamic_cast<FormatHex*>(output_format) != NULL) {
        return;
    }

    // Random input works with any output format.
    if(dynamic_cast<FormatRandom*>(input_format) != NULL) {
        return;
    }

    // Hex input works with any output format.
    if(dynamic_cast<FormatHex*>(input_format) != NULL) {
        return;
    }

    argp_error(state, "Input format %s cannot be used with output format %s", 
        input_format->name.c_str(), output_format->name.c_str());
}

void Params::validate_ints_specific() {
    auto f = dynamic_cast<FormatInts*>(output_format);
    if(f != NULL) {
        int low = f->low;
        int high = f->high;
        if(!raw.ints_low.empty()) {
            low = stoi(raw.ints_low);
        }
        if(!raw.ints_high.empty()) {
            high = stoi(raw.ints_high);
        }
        if(!(0 <= low && low < high && high <= 255)) {
            argp_error(state, "--low and --high must specify a range in [0-255].");
        }
        f->low = low;
        f->high = high;
    } else {
        if(!raw.ints_low.empty()) {
            argp_error(state, "Option --low can only be used with the \"ints\" output format.");
        }
        if(!raw.ints_high.empty()) {
            argp_error(state, "Option --high can only be used with the \"ints\" output format.");
        }
    }
}

void Params::validate_bip39_specific() {
    if(dynamic_cast<FormatBIP39*>(output_format) == NULL) { return; }
    if(!FormatBIP39::is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [12-32] and even.");
    }
}

group_descriptor Params::parse_group_spec(const string &string) {
    size_t threshold;
    size_t count;
    auto items = sscanf(string.c_str(), "%zd-of-%zd", &threshold, &count);
    if(items != 2) {
        argp_error(state, "Could not parse group specifier: \"%s\"", string.c_str());
    }
    if(!(0 < threshold && threshold <= count && count <= 16)) {
        argp_error(state, "Invalid group specifier \"%s\": 1 <= N <= M <= 16", string.c_str());
    }
    if(count > 1 && threshold == 1) {
        argp_error(state, "Invalid group specifier. 1-of-M groups where M > 1 are not supported.");
    }
    group_descriptor g;
    g.threshold = threshold;
    g.count = count;
    g.passwords = NULL;
    return g;
}

void Params::validate_slip39_specific() {
    auto raw_groups_count = raw.slip39_groups.size();

    auto of = dynamic_cast<FormatSLIP39*>(output_format);
    if(of == NULL) {
        if(raw_groups_count > 0) {
            argp_error(state, "Option --group can only be used with the \"slip39\" output format.");
        }
        if(!raw.slip39_groups_threshold.empty()) {
            argp_error(state, "Option --group-threshold can only be used with the \"slip39\" output format.");
        }
        return;
    }

    if(!FormatSLIP39::is_seed_length_valid(count)) {
        argp_error(state, "For BIP39 COUNT must be in [16-32] and even.");
    }

    vector<group_descriptor> groups;
    if(raw_groups_count > MAX_GROUPS) {
        argp_error(state, "There must be no more than %d groups.", MAX_GROUPS);
    } else if(raw_groups_count == 0) {
        groups.push_back( {1, 1} );
    } else {
        for(auto g: raw.slip39_groups) {
            auto group = parse_group_spec(g);
            groups.push_back(group);
        }
    }
    
    int groups_threshold;
    if(raw.slip39_groups_threshold.empty()) {
        groups_threshold = 1;
    } else {
        groups_threshold = stoi(raw.slip39_groups_threshold);
    }
    if(!(0 < groups_threshold && groups_threshold <= groups.size())) {
        argp_error(state, "Group threshold must be <= the number of groups.");
    }

    of->groups_threshold = groups_threshold;
    of->groups = groups;
}

void Params::validate_input() {
    // Every input method takes arguments except random.
    if (dynamic_cast<FormatRandom*>(input_format) != NULL) {
        if (!raw.args.empty()) {
            argp_error(state, "Do not provide arguments when using the random (default) input format.");
        }
    } else {
        if (raw.args.empty()) {
            read_args_from_stdin();
        }
        input = raw.args;
        if(input.empty()) {
            argp_error(state, "No input provided.");
        }
    }
}

void Params::validate_count_for_input_format() {
    if (dynamic_cast<FormatHex*>(input_format) != NULL) {
        if (!raw.count.empty()) {
            argp_error(state, "The --count option is not available for hex input.");
        }
    }
}

void Params::validate() {
    validate_count();
    validate_deterministic();
    validate_input_format();
    validate_count_for_input_format();
    validate_output_format();
    validate_output_for_input();
    validate_ints_specific();
    validate_bip39_specific();
    validate_slip39_specific();
    validate_input();
}

void Params::read_args_from_stdin() {
    string line;
    while(getline(cin, line)) {
        raw.args.push_back(line);
    }
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    try {
        auto p = static_cast<Params*>(state->input);
        p->state = state;
        auto& raw = p->raw;

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
                p->validate();
            }
            break;
        }
    } catch(exception &e) {
        argp_error(state, "%s", e.what());
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
    {"group-threshold", 't', "1-16", 0, "The number of groups that must meet their threshold (default: 1)"},
    {"group", 'g', "M-of-N", 0, "The group specification (default: 1-of-1)"},
    {"The --group option may appear more than once.", 0, 0, OPTION_DOC, 0},
    {"M < N", 0, 0, OPTION_DOC, 0},
    {"The group threshold must be <= the number of group specifications.", 0, 0, OPTION_DOC, 0},

    {0, 0, 0, 0, "Deterministic Random Numbers:", 3},
    {"deterministic", 'd', "SEED", 0, "Use a deterministic random number generator with the given seed."},

    { 0 }
};

auto argp_program_version = PACKAGE_VERSION;
const char* argp_program_bug_address = "ChristopherA@LifeWithAlacrity.com";

auto doc = "Converts cryptographic seeds between various forms.";
struct argp argp = { options, parse_opt, "INPUT", doc };

Params* Params::parse( int argc, char *argv[] ) {
    auto p = new Params();
    argp_parse( &argp, argc, argv, 0, 0, p );
    return p;
}

string Params::get_one_argument() {
    if(input.size() != 1) {
        throw runtime_error("Only one argument accepted.");
    }
    return input[0];
}

string Params::get_combined_arguments() {
    return join(input, " ");
}

vector<string> Params::get_multiple_arguments() {
    return input;
}
