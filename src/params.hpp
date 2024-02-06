//
//  params.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <optional>
#include <bc-sskr/bc-sskr.h>
#include <bc-ur/bc-ur.hpp>

#include "random.hpp"
#include "utils.hpp"

#define MAX_GROUPS 16
#define MAX_RAW_GROUPS (MAX_GROUPS + 1)

#define MAX_ARGS 256

class RawParams {
public:
    RawParams() { }

    std::string input_format;
    std::string output_format;
    std::string count;

    std::string ints_low;
    std::string ints_high;

    std::string random_deterministic;
    bool is_ur = false;
    std::string max_fragment_length;
    std::string fountain_parts;

    std::string sskr_groups_threshold;
    StringVector sskr_groups;

    StringVector args;
};

class Format;

class Params {
public:
    Params() { }
    ~Params();

    Format* input_format = NULL;
    Format* output_format = NULL;

    bool is_ur_out = false;
    bool is_ur_in = false;
    std::vector<ur::UR> ur_shares;
    size_t max_fragment_length = 0;
    std::optional<size_t> fountain_parts;

    StringVector shares;

    StringVector input;
    std::string output;

    ByteVector seed;

    size_t count = 0;

    char* deterministic_seed = NULL;
    random_generator rng = NULL;

    void validate();

    RawParams raw;

    struct argp_state* state = nullptr;

    static Params* parse( int argc, char *argv[] );
    void read_args_from_stdin();

    std::string get_one_argument();
    std::string get_combined_arguments();
    StringVector get_multiple_arguments();

    void set_ur_output(const ByteVector& cbor, const std::string& type);

private:
    sskr_group_descriptor parse_group_spec(const std::string &string);

    void validate_count();
    void validate_deterministic();
    void validate_input_format();
    void validate_count_for_input_format();
    void validate_output_format();
    void validate_ints_specific();
    void validate_bip39_specific();
    void validate_sskr_specific();
    void validate_input();
    void validate_ur();
};
