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

typedef struct format_t format;

typedef struct params_t {
    format* input_format;
    char* input;

    format* output_format;
    char* output;

    uint8_t* seed;
    size_t seed_len;
    size_t count;
} params;

params* params_parse( int argc, char *argv[] );
void params_dispose(params*);

#endif /* PARAMS_H */
