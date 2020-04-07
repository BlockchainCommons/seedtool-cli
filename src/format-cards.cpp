//
//  format-cards.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-cards.hpp"

#include <strings.h>

#include "params.hpp"
#include "utils.hpp"

static const std::string card_suits[] = { "c", "d", "h", "s" };
static const std::string card_ranks[] = { "a", "2", "3", "4", "5", "6", "7", "8", "9", "t", "j", "q", "k"};

static std::string to_card(size_t n) {
    if(n > 51) { return NULL; }
    std::string buf;
    size_t rank = n % 13;
    size_t suit = n / 13;
    buf.append(card_ranks[rank]);
    buf.append(card_suits[suit]);
    return buf;
}

static void format_cards_process_output(format* f, params* p) {
    p->output = data_to_alphabet(p->seed, 52, to_card);
}

static void format_cards_dispose(format* f) {
    free(f);
}

format* format_cards_new() {
    format* f = (format*)calloc(sizeof(format), 1);
    f->key = format_key_cards;
    f->name = "cards";
    f->process_output = format_cards_process_output;
    f->dispose = format_cards_dispose;
    return f;
}
