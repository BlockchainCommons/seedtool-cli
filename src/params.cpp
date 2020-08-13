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
        if(raw.input_format == "ur") {
            is_ur_in = true;
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
                case Format::Key::bytewords: input_format = new FormatBytewords(); break;
                case Format::Key::bytewords_uri: input_format = new FormatBytewordsURI(); break;
                case Format::Key::bytewords_minimal: input_format = new FormatBytewordsMinimal(); break;
                default:
                    argp_error(state, "Unknown input format: %s", raw.input_format.c_str());
                    break;
            }
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
            case Format::Key::bytewords: output_format = new FormatBytewords(); break;
            case Format::Key::bytewords_uri: output_format = new FormatBytewordsURI(); break;
            case Format::Key::bytewords_minimal: output_format = new FormatBytewordsMinimal(); break;
            default:
                argp_error(state, "Unknown output format: %s", raw.output_format.c_str());
                break;
        }
    }
}

void Params::validate_output_for_input() {
    // Any input format works with hex output format.
    if(is_hex(output_format)) {
        return;
    }

    // Any input format works with any Bytewords output format.
    if(is_bytewords_any(output_format)) {
        return;
    }

    // Random input works with any output format.
    if(is_random(input_format)) {
        return;
    }

    // Hex input works with any output format.
    if(is_hex(input_format)) {
        return;
    }

    // Any Bytewords input works with any output format.
    if(is_bytewords_any(input_format)) {
        return;
    }

    // BIP39 UR input works with BIP39 output format.
    if(is_ur_in && is_bip39(input_format) && is_bip39(output_format)) {
        return;
    }

    // SLIP39 UR input works with SLIP39 output format.
    if(is_ur_in && is_slip39(input_format) && is_slip39(output_format)) {
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
    if(!is_bip39(output_format)) { return; }
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
    if (is_random(input_format)) {
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

        if(is_ur_in) {
            auto decoder = ur::URDecoder();
            for(auto part: input) {
                decoder.receive_part(part);
            }
            if(!decoder.is_complete()) {
                argp_error(state, "Incomplete UR parts.");
            } else if(decoder.is_failure()) {
                argp_error(state, "%s", decoder.result_error().what());
            } else {
                ur = decoder.result_ur();
                if(ur->type() == "crypto-seed") {
                    input_format = new FormatHex();
                } else if(ur->type() == "crypto-bip39") {
                    input_format = new FormatBIP39();
                } else if(ur->type() == "crypto-slip39") {
                    input_format = new FormatSLIP39();
                } else {
                    argp_error(state, "Unknown UR type.");
                }
            }
        }
    }
}

void Params::validate_count_for_input_format() {
    if (is_hex(input_format)) {
        if (!raw.count.empty()) {
            argp_error(state, "The --count option is not available for hex input.");
        }
    } else if (is_bytewords_any(input_format)) {
        if (!raw.count.empty()) {
            argp_error(state, "The --count option is not available for Bytewords input.");
        }
    }
}

void Params::validate_ur() {
    // The --ur option is only available for hex, BIP39 and SLIP39 output.
    if(raw.is_ur) {
        if(is_ur_in) {
            argp_error(state, "The --ur option may not be combined with the --in ur input method.");
        }

        is_ur_out = true;

        if(raw.max_fragment_length.empty()) {
            max_fragment_length = 2500;
        } else {
            max_fragment_length = stoi(raw.max_fragment_length);
        }

        if(max_fragment_length < 10) {
            argp_error(state, "MAX_FRAGMENT_LENGTH must be >= 10.");
            return;
        }

        if(!raw.fountain_parts.empty()) {
            fountain_parts = stoi(raw.fountain_parts);
        }

        if(is_hex(output_format)) { return; }
        if(is_bip39(output_format)) { return; }
        if(is_slip39(output_format)) { return ; }

        argp_error(state, "The --ur option is only available for hex, BIP39 and SLIP39 output.");
    }
}

void Params::validate() {
    validate_count();
    validate_deterministic();
    validate_input_format();
    validate_input();
    validate_count_for_input_format();
    validate_output_format();
    validate_output_for_input();
    validate_ints_specific();
    validate_bip39_specific();
    validate_slip39_specific();
    validate_ur();
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
            case 'c': raw.count = arg; break;
            case 'd': raw.random_deterministic = arg; break;
            case 'g': raw.slip39_groups.push_back(arg); break;
            case 'h': raw.ints_high = arg; break;
            case 'i': raw.input_format = arg; break;
            case 'l': raw.ints_low = arg; break;
            case 'o': raw.output_format = arg; break;
            case 't': raw.slip39_groups_threshold = arg; break;
            case 'u': raw.is_ur = true; raw.max_fragment_length = arg != NULL ? arg : ""; break;
            case 'p': raw.fountain_parts = arg; break;
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
    {"in", 'i', "random|hex|btw|btwu|btwm|bits|cards|dice|base6|base10|ints|bip39|slip39|ur", 0, "The input format (default: random)"},
    {"out", 'o', "hex|btw|btwu|btwm|bits|cards|dice|base6|base10|ints|bip39|slip39", 0, "The output format (default: hex)"},
    {"count", 'c', "1-64", 0, "The number of output units (default: 32)"},
    {"ur", 'u', "MAX_FRAGMENT_LENGTH", OPTION_ARG_OPTIONAL, "Encode output as a Uniform Resource (UR). If necessary the UR will be segmented into parts with fragments no larger than MAX_FRAGMENT_LENGTH."},
    {"parts", 'p', "FOUNTAIN_PARTS", 0, "For multi-part URs, the number of additional UR parts above the minimum to generate using fountain encoding."},

    {0, 0, 0, 0, "ints Input and Output Options:", 1},
    {"low", 'l', "0-254", 0, "The lowest int returned (default: 1)"},
    {"high", 'h', "1-255", 0, "The highest int returned (default: 9)"},
    {"low < high", 0, 0, OPTION_NO_USAGE, 0},

    {0, 0, 0, 0, "SLIP39 Output Options:", 2},
    {"group-threshold", 't', "1-16", 0, "The number of groups that must meet their threshold (default: 1)"},
    {"group", 'g', "M-of-N", 0, "The group specification (default: 1-of-1)"},
    {"The --group option may appear more than once.", 0, 0, OPTION_NO_USAGE, 0},
    {"M < N", 0, 0, OPTION_NO_USAGE, 0},
    {"The group threshold must be <= the number of group specifications.", 0, 0, OPTION_NO_USAGE, 0},

    {0, 0, 0, 0, "Deterministic Random Numbers:", 3},
    {"deterministic", 'd', "SEED", 0, "Use a deterministic random number generator with the given seed."},

    { 0 }
};

auto argp_program_version = PACKAGE_VERSION;
const char* argp_program_bug_address = "ChristopherA@BlockchainCommons.com. © 2020 Blockchain Commons";

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

string_vector Params::get_multiple_arguments() {
    return input;
}

void Params::set_ur_output(const byte_vector& cbor, const string& type) {
    //cout << data_to_hex(cbor) << endl;
    auto u = ur::UR(type, cbor);
    auto encoder = ur::UREncoder(u, max_fragment_length);
    size_t seq_len;
    if(encoder.is_single_part()) {
        seq_len = 1;
    } else {
        seq_len = encoder.seq_len();
        if(fountain_parts.has_value()) {
            seq_len += *fountain_parts;
        }
    }
    string_vector parts;
    for(auto i = 0; i < seq_len; i++) {
        parts.push_back(encoder.next_part());
    }
    output = join(parts, "\n");
}
