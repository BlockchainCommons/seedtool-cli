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
#include <bc-slip39/bc-slip39.h>
#include <bc-ur/bc-ur.hpp>

#include "random.hpp"
#include "utils.hpp"

#define MAX_GROUPS 16
#define MAX_RAW_GROUPS (MAX_GROUPS + 1)

#define MAX_ARGS 256

class RawParams {
public:
    std::string input_format;
    std::string output_format;
    std::string count;

    std::string ints_low;
    std::string ints_high;

    std::string random_deterministic;
    bool is_ur;
    std::string max_part_length;

    std::string slip39_groups_threshold;
    string_vector slip39_groups;

    string_vector args;
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
    std::optional<ur::UR> ur;
    size_t max_part_length = 0;

    string_vector shares;

    string_vector input;
    std::string output;

    byte_vector seed;

    size_t count = 0;

    char* deterministic_seed = NULL;
    random_generator rng = NULL;

    void validate();

    RawParams raw;

    struct argp_state* state;

    static Params* parse( int argc, char *argv[] );
    void read_args_from_stdin();

    std::string get_one_argument();
    std::string get_combined_arguments();
    string_vector get_multiple_arguments();

    void set_ur_output(const byte_vector& cbor, const std::string& type);
    
private:
    group_descriptor parse_group_spec(const std::string &string);

    void validate_count();
    void validate_deterministic();
    void validate_input_format();
    void validate_count_for_input_format();
    void validate_output_format();
    void validate_output_for_input();
    void validate_ints_specific();
    void validate_bip39_specific();
    void validate_slip39_specific();
    void validate_input();
    void validate_ur();
};
