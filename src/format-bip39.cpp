//
//  format-bip39.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bip39.hpp"

#include <strings.h>
#include <stdexcept>

#include <bc-bip39/bc-bip39.h>
#include <bc-ur/bc-ur.hpp>

#include "params.hpp"
#include "utils.hpp"
#include "cbor-utils.hpp"

using namespace std;

bool FormatBIP39::is_seed_length_valid(size_t seed_len) {
    if(!(12 <= seed_len && seed_len <= 32)) { return false; }
    if(seed_len % 2 != 0) { return false; }
    return true;
}

void FormatBIP39::process_input(Params* p) {
    string input;

    if(p->is_ur_in) {
        auto& ur = p->ur_shares.front();
        auto pos = ur.cbor().begin();
        const auto end = ur.cbor().end();

        StringVector strings;
        typedef ur::ByteVector::const_iterator Iter;
        auto f = [&strings](Iter& pos, Iter end) {
            decode_string_array(pos, end, strings);
        };
        decode_dict_with_birthdate(pos, end, f);
        input = join(strings, " ");
    } else {
        input = p->get_combined_arguments();
    }

    ByteVector buf;
    buf.resize(300);
    auto len = bip39_secret_from_mnemonics(input.c_str(), buf.data(), buf.size());
    if(len == 0) {
        throw runtime_error("Invalid BIP39 word sequence.");
    }
    buf.resize(len);
    p->seed = buf;
}

void FormatBIP39::process_output(Params* p) {
    if(!is_seed_length_valid(p->seed.size())) {
        throw runtime_error("Invalid seed length for BIP39. Must be in [12-32] and even.");
    }

    size_t max_mnemonics_len = 300;
    char mnemonics[max_mnemonics_len];
    size_t len = bip39_mnemonics_from_secret(p->seed.data(), p->seed.size(), mnemonics, max_mnemonics_len);
    string s = mnemonics;

    if(p->is_ur_out) {
        auto words = split(s, ' ');
        ByteVector encoded_words;
        encode_string_array(encoded_words, words);
        ByteVector dict;
        encode_dict_with_birthdate(dict, encoded_words, false);
        p->set_ur_output(dict, "crypto-bip39");
    } else {
        p->output = s;
    }
}
