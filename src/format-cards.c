//
//  format-cards.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-cards.h"

#include <strings.h>

#include "params.h"
#include "utils.h"

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

void format_cards_process_output(format* this, params* p) {
    p->output = data_to_alphabet(p->seed, p->seed_len, 52, to_card);
}

static void format_cards_dispose(format* this) {
    free(this);
}

format* format_cards_new() {
    format* f = calloc(sizeof(format), 1);
    f->key = format_key_cards;
    f->name = "cards";
    f->process_output = format_cards_process_output;
    f->dispose = format_cards_dispose;
    return f;
}
