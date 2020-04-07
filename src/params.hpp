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

class raw_params {
public:
    const char* input_format = NULL;
    const char* output_format = NULL;
    const char* count = NULL;

    const char* ints_low = NULL;
    const char* ints_high = NULL;

    const char* random_deterministic = NULL;

    const char* slip39_groups_threshold = 0;
    const char* slip39_groups[MAX_RAW_GROUPS];
    size_t slip39_groups_count = 0;

    const char* args[MAX_ARGS];
    size_t args_count = 0;

    void add_group(const char* g);
    void add_arg(const char* a);
};

struct format;

class params {
public:
    params() { }
    ~params();

    raw_params* raw = new raw_params();

    struct format* input_format = NULL;
    struct format* output_format = NULL;

    char* input = NULL;
    std::string output;

    std::vector<uint8_t> seed;

    size_t count = 0;

    char* deterministic_seed = NULL;
    random_generator rng = NULL;

    void validate(struct argp_state* state);

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

params* params_parse( int argc, char *argv[] );
// void params_dispose(struct params*);

#endif /* PARAMS_HPP */
