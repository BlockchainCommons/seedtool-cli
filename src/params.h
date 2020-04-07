//
//  params.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef PARAMS_H
#define PARAMS_H

#include <stdlib.h>
#include <stdint.h>
#include "random.h"

#define MAX_GROUPS 16
#define MAX_RAW_GROUPS (MAX_GROUPS + 1)

#define MAX_ARGS 256

typedef struct raw_params_t {
    const char* input_format;
    const char* output_format;
    const char* count;

    const char* ints_low;
    const char* ints_high;

    const char* random_deterministic;

    const char* slip39_groups_threshold;
    const char* slip39_groups[MAX_RAW_GROUPS];
    size_t slip39_groups_count;

    const char* args[MAX_ARGS];
    size_t args_count;
} raw_params;

typedef struct format_t format;

typedef struct params_t {
    format* input_format;
    char* input;

    format* output_format;
    char* output;

    uint8_t* seed;
    size_t seed_len;
    size_t count;

    char* deterministic_seed;
    random_generator rng;

    raw_params* raw;
} params;

params* params_parse( int argc, char *argv[] );
void params_dispose(params*);

#endif /* PARAMS_H */
