//
//  format-hex.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-hex.hpp"

#include <iostream>
#include <vector>

#include <stdexcept>
#include <set>

#include "params.hpp"
#include "utils.hpp"
#include "cbor.hpp"
#include "cbor-lite/codec.h"

using namespace std;

void FormatHex::process_input(Params* p) {
    auto input = p->get_one_argument();

    // Currently compatible with with https://iancoleman.io/bip39/
    // ONLY in "raw entropy" mode.

    if(p->is_ur_in) {
        const UR& ur = *p->ur;
        auto pos = ur.cbor.begin();
        const auto end = ur.cbor.end();

        byte_vector bytes;
        typedef decltype(ur.cbor)::const_iterator Iter;
        auto f = [&bytes](Iter& pos, Iter end) {
            decode_byte_string(pos, end, bytes);
        };
        decode_dict_with_birthdate(pos, end, f);

        p->seed = bytes;
    } else {
        p->seed = hex_to_data(input);
    }
}

void FormatHex::process_output(Params* p) {
    if(p->is_ur_out) {
        byte_vector byte_string;
        encode_byte_string(byte_string, p->seed);
        byte_vector dict;
        encode_dict_with_birthdate(dict, byte_string);
        p->set_ur_output(dict, "crypto-seed");
    } else {
        p->output = data_to_hex(p->seed);
    }
}
