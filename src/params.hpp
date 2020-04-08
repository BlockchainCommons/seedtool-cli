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
#include <bc-slip39/bc-slip39.h>

#include "random.hpp"

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

    std::string slip39_groups_threshold;
    std::vector<std::string> slip39_groups;

    std::vector<std::string> args;
};

class Format;

class Params {
public:
    Params() { }
    ~Params();

    Format* input_format = NULL;
    Format* output_format = NULL;

    std::vector<std::string> input;
    std::string output;

    std::vector<uint8_t> seed;

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
    std::vector<std::string> get_multiple_arguments();

private:
    group_descriptor parse_group_spec(const std::string &string);

    void validate_count();
    void validate_deterministic();
    void validate_input_format();
    void validate_output_format();
    void validate_output_for_input();
    void validate_ints_specific();
    void validate_bip39_specific();
    void validate_slip39_specific();
    void validate_input();
};