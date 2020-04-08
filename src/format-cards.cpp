//
//  format-cards.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-cards.hpp"

#include <stdexcept>
#include <cctype>

#include "params.hpp"
#include "utils.hpp"
#include "random.hpp"

using namespace std;

// https://en.wikipedia.org/wiki/High_card_by_suit
static const vector<char> card_suits = { 'd', 'c', 'h', 's' };
static const vector<char> card_ranks = { 'a', '2', '3', '4', '5', '6', '7', '8', '9', 't', 'j', 'q', 'k'};

static size_t parse_rank(char c) {
    c = tolower(c);
    for(size_t i = 0; i < card_ranks.size(); i++) {
        if(c == card_ranks[i]) {
            return i;
        }
    }    
    throw runtime_error("Invalid card rank. Allowed: [A,2-9,T,J,Q,K]");
}

static size_t parse_suit(char c) {
    c = tolower(c);
    for(size_t i = 0; i < card_suits.size(); i++) {
        if(c == card_suits[i]) {
            return i;
        }
    }    
    throw runtime_error("Invalid card rank. Allowed: [D,C,H,S]");
}

vector<uint8_t> cards_to_data(const string& cards) {
    vector<uint8_t> result;

    auto len = cards.length();
    if(len % 2 != 0) {
        throw runtime_error("Cards string must have even number of characters.");
    }
    auto count = len / 2;
    result.reserve(count);
    for(auto i = 0; i < count; i++) {
        auto rank = parse_rank(cards[i * 2]);
        auto suit = parse_suit(cards[i * 2 + 1]);
        auto n = suit * 13 + rank;
        result.push_back(n);
    }

    return result;
}

void FormatCards::process_input(Params* p) {
    auto input = p->get_one_argument();
    auto entropy = cards_to_data(input);
    p->seed = deterministic_random(entropy, p->count);
}

static string to_card(size_t n) {
    if(n > 51) { return NULL; }
    string buf;
    size_t rank = n % 13;
    size_t suit = n / 13;
    buf += card_ranks[rank];
    buf += card_suits[suit];

    // test value round trip
    auto v = cards_to_data(buf);
    assert(v.size() == 1);
    assert(v[0] == n);

    return buf;
}

void FormatCards::process_output(Params* p) {
    p->output = data_to_alphabet(p->seed, 52, to_card);
}
