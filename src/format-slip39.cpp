//
//  format-slip39.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-slip39.hpp"

#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include "params.hpp"
#include "utils.hpp"

bool FormatSLIP39::is_seed_length_valid(size_t seed_len) {
    if(!(16 <= seed_len && seed_len <= 32)) { return false; }
    if(seed_len % 2 != 0) { return false; }
    return true;
}

void FormatSLIP39::process_input(Params* p) {
}

void FormatSLIP39::process_output(Params* p) {
    if(!is_seed_length_valid(p->seed.size())) { return; }

    const char* password = "";

    uint8_t iteration_exponent = 0;

    uint32_t words_in_each_share = 0;
    size_t shares_buffer_size = 1024;
    uint16_t shares_buffer[shares_buffer_size];

    int share_count = slip39_generate(
        groups_threshold, &groups[0], groups.size(),
        &p->seed[0], p->seed.size(), password,
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

FormatSLIP39::FormatSLIP39() : Format(Format::Key::slip39, "slip39") {
    groups_threshold = 1;
    groups = {{1, 1}};
}
