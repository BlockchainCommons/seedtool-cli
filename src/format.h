//
//  format.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef FORMAT_H
#define FORMAT_H

typedef struct params_t params;
typedef struct format_t format;

typedef void (*format_func)(format*);
typedef void (*format_processor)(format*, params*);

typedef enum format_key_t {
    format_key_unknown = -1,
    format_key_random,
    format_key_hex,
    format_key_bits,
    format_key_cards,
    format_key_dice,
    format_key_base6,
    format_key_base10,
    format_key_ints,
    format_key_bip39,
    format_key_slip39
} format_key;

typedef struct format_t {
    format_key key;
    const char* name;
    format_processor process_input;
    format_processor process_output;
    format_func validate;
    format_func dispose;
    void* format_options;
} format;

void format_dispose(format* f);

format_key format_key_for_string(const char* arg);

#endif /* FORMAT_H */
