//
//  format-bip39.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-bip39.hpp"

#include <strings.h>
#include <bc-bip39/bc-bip39.h>
#include <stdexcept>

#include "params.hpp"
#include "utils.hpp"

using namespace std;

bool FormatBIP39::is_seed_length_valid(size_t seed_len) {
    if(!(12 <= seed_len && seed_len <= 32)) { return false; }
    if(seed_len % 2 != 0) { return false; }
    return true;
}

void FormatBIP39::process_input(Params* p) {
    auto input = p->get_combined_arguments();
    vector<uint8_t> buf;
    buf.resize(300);
    auto len = bip39_secret_from_mnemonics(input.c_str(), &buf[0], buf.size());
    if(len == 0) {
        throw runtime_error("Invalid BIP39 word sequence.");
    }
    buf.resize(len);
    p->seed = buf;
}

void FormatBIP39::process_output(Params* p) {
    if(!is_seed_length_valid(p->seed.size())) { return; }

    size_t max_mnemonics_len = 300;
    char mnemonics[max_mnemonics_len];
    size_t len = bip39_mnemonics_from_secret(&p->seed[0], p->seed.size(), mnemonics, max_mnemonics_len);
    char* string = (char*)malloc(len);
    strcpy(string, mnemonics);
    p->output = string;
}
