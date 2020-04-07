//
//  generators.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "generators.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <bc-crypto-base/bc-crypto-base.h>
#include <bc-bip39/bc-bip39.h>

#include "utils.h"
#include "random.h"

static uint8_t* random_seed(size_t count) {
    uint8_t* bytes = malloc(count);
    crypto_random(bytes, count);
    return bytes;
}

static uint8_t* random_data_in_base(size_t base, size_t count) {
    if (!(2 <= base && base <= 256)) {
        return NULL;
    }
    if (count == 0) {
        return NULL;
    }

    uint8_t* bytes = random_seed(count);
    if (base < 256) {
        for (int i = 0; i < count; i++) {
            float f = bytes[i] / 255.0;
            uint8_t b = roundf(f * (base - 1));
            bytes[i] = b;
        }
    }
    return bytes;
}

char* random_hex(size_t count) {
    uint8_t* bytes = random_data_in_base(256, count);
    char* string = data_to_hex(bytes, count);
    free(bytes);
    return string;
}

static char* random_base(size_t base, size_t count, char* (to_alphabet)(size_t)) {
    uint8_t* bytes = random_data_in_base(base, count);
    char* string = data_to_alphabet(bytes, count, base, to_alphabet);
    free(bytes);
    return string;
}

static const char* card_suits[] = { "c", "d", "h", "s" };
static const char* card_ranks[] = { "a", "2", "3", "4", "5", "6", "7", "8", "9", "t", "j", "q", "k"};

static char* to_card(size_t n) {
    if(n > 51) { return NULL; }
    char* buf = malloc(3);
    size_t rank = n % 13;
    size_t suit = n / 13;
    const char* rank_string = card_ranks[rank];
    const char* suit_string = card_suits[suit];
    strcpy(buf, rank_string);
    strcat(buf, suit_string);
    return buf;
}

char* random_cards(size_t count) {
    return random_base(52, count, to_card);
}

static void int_to_string(int n, char* out) {
    sprintf(out, "%d", n);
}

char* random_ints(size_t low, size_t high, size_t count, const char* separator) {
    if(low >= high) { return NULL; }
    if(high + low - 1 > 255) { return NULL; }

    size_t base = high - low + 1;
    uint8_t* data = random_data_in_base(base, count);

    size_t separator_len = strlen(separator);

    size_t len = 0;
    char buf[10];
    for(int i = 0; i < count; i++) {
        data[i] += low;

        if(i > 0) {
            len += separator_len;
        }
        size_t d = data[i];
        int_to_string(d, buf);
        len += strlen(buf) + separator_len;
    }

    char* string = malloc(len + 1);
    string[0] = '\0';
    for(int i = 0; i < count; i++) {
        if(i > 0) {
            strcat(string, separator);
        }
        int_to_string(data[i], buf);
        strcat(string, buf);
    }

    return string;
}

char* random_bip39(size_t secret_len) {
    if(!(4 <= secret_len && secret_len <= 32)) { return NULL; }
    if(secret_len % 2 != 0) { return NULL; }

    uint8_t* secret = random_seed(secret_len);

    size_t max_mnemonics_len = 300;
    char mnemonics[max_mnemonics_len];
    size_t len = bip39_mnemonics_from_secret(secret, secret_len, mnemonics, max_mnemonics_len);
    char* string = malloc(len);
    strcpy(string, mnemonics);

    free(secret);

    return string;
}

char* random_slip39(size_t secret_len, size_t group_threshold, const group_descriptor* groups, size_t group_count) {
    if(!(16 <= secret_len && secret_len <= 32)) { return NULL; }
    if(secret_len % 2 != 0) { return NULL; }

    uint8_t* secret_data = random_seed(secret_len);

    char* password = "";

    uint8_t iteration_exponent = 0;

    uint32_t words_in_each_share = 0;
    size_t shares_buffer_size = 1024;
    uint16_t shares_buffer[shares_buffer_size];

    int share_count = slip39_generate(
        group_threshold, groups, group_count, secret_data, secret_len, password,
        iteration_exponent, &words_in_each_share, shares_buffer,
        shares_buffer_size, crypto_random);

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

    char* all_strings = malloc(len + 1);
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

    return all_strings;
}