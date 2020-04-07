//
//  params.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>

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

    std::string input;
    std::string output;

    std::vector<uint8_t> seed;

    size_t count = 0;

    char* deterministic_seed = NULL;
    random_generator rng = NULL;

    void validate(struct argp_state* state);

    RawParams raw;

    static Params* parse( int argc, char *argv[] );

private:
    void validate_count(struct argp_state* state);
    void validate_deterministic(struct argp_state* state);
    void validate_input_format(struct argp_state* state);
    void validate_output_format(struct argp_state* state);
    void validate_output_for_input(struct argp_state* state);
    void validate_ints_specific(struct argp_state* state);
    void validate_bip39_specific(struct argp_state* state);
    void validate_slip39_specific(struct argp_state* state);
};

#endif /* PARAMS_HPP */
