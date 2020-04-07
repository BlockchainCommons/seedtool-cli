//
//  format-cards.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-cards.hpp"
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

void FormatCards::process_input(Params* p) {
}

void FormatCards::process_output(Params* p) {
    p->output = data_to_alphabet(p->seed, 52, to_card);
}
