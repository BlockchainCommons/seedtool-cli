//
//  format-slip39.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-slip39.h"

#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include "params.h"
#include "utils.h"

bool format_slip39_is_seed_length_valid(size_t seed_len) {
    if(!(16 <= seed_len && seed_len <= 32)) { return false; }
    if(seed_len % 2 != 0) { return false; }
    return true;
}

typedef struct format_slip39_options_t {
    group_descriptor groups[16];
    size_t groups_count;
    size_t groups_threshold;
} format_slip39_options;

void format_slip39_process_output(format* f, params* p) {
    if(!format_slip39_is_seed_length_valid(p->seed_len)) { return; }

    format_slip39_options* opts = (format_slip39_options*)f->format_options;

    const char* password = "";

    uint8_t iteration_exponent = 0;

    uint32_t words_in_each_share = 0;
    size_t shares_buffer_size = 1024;
    uint16_t shares_buffer[shares_buffer_size];

    int share_count = slip39_generate(
        opts->groups_threshold, opts->groups, opts->groups_count,
        p->seed, p->seed_len, password,
        iteration_exponent, &words_in_each_share, shares_buffer,
        shares_buffer_size, p->rng);

    assert(share_count > 0);

    char* strings[share_count];
    size_t len = 0;
    for (int i = 0; i < share_count; i++) {
        if(i > 0) {
            len += 1;
        }
        uint16_t* words = shares_buffer + (i * words_in_each_share);
        strings[i] = slip39_strings_for_words(words, words_in_each_share);
        len += strlen(strings[i]);
    }

    char* all_strings = (char*)malloc(len + 1);
    all_strings[0] = '\0';
    for(int i = 0; i < share_count; i++) {
        if(i > 0) {
            strcat(all_strings, "\n");
        }
        strcat(all_strings, strings[i]);
    }

    for (int i = 0; i < share_count; i++) {
        free(strings[i]);
    }

    p->output = all_strings;
}

static void format_slip39_dispose(format* f) {
    free(f->format_options);
    free(f);
}

format* format_slip39_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_slip39;
    f->name = "slip39";

    format_slip39_options* opts = (format_slip39_options*)calloc(sizeof(format_slip39_options), 1);
    opts->groups_count = 1;
    opts->groups_threshold = 1;
    opts->groups[0] = (group_descriptor) {1, 1};

    f->format_options = opts;
    f->process_output = format_slip39_process_output;
    f->dispose = format_slip39_dispose;
    return f;
}

void format_slip39_set_groups_threshold(format* f, size_t groups_threshold) {
    format_slip39_options* opts = (format_slip39_options*)f->format_options;
    opts->groups_threshold = groups_threshold;
}

void format_slip39_set_groups(format* f, const group_descriptor* groups, size_t groups_count) {
    format_slip39_options* opts = (format_slip39_options*)f->format_options;
    opts->groups_count = groups_count;
    for(int i = 0; i < groups_count; i++) {
        opts->groups[i] = groups[i];
    }
}
