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
#include <bc-slip39/bc-slip39.h>

typedef enum format_t {
    format_random,
    format_hex,
    format_bits,
    format_cards,
    format_dice,
    format_base6,
    format_base10,
    format_ints,
    format_bip39,
    format_slip39
} format;

typedef struct params_t {
    format input_format;
    void (*process_input)(struct params_t*);
    char* input;

    format output_format;
    void (*process_output)(struct params_t*);
    char* output;

    uint8_t* seed;
    size_t seed_len;
    size_t count;

    char* deterministic_seed;
    void (*rng)(uint8_t*, size_t);

    char* error;

    // ints-specific
    size_t low;
    size_t high;

    // SLIP39-specific
    group_descriptor* groups;
    size_t groups_count;
    size_t groups_threshold;
} params;

params* parse_params( int argc, char *argv[] );

params* new_params();
void dispose_params(params*);

#endif /* PARAMS_H */
