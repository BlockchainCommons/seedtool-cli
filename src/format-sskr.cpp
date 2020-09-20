//
//  format-sskr.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-sskr.hpp"

#include <stdexcept>
#include <iostream>

#include "params.hpp"
#include "cbor-utils.hpp"
#include <bc-ur/bc-ur.hpp>

using namespace std;

bool FormatSSKR::is_seed_length_valid(size_t seed_len) {
    if(!(16 <= seed_len && seed_len <= 32)) { return false; }
    if(seed_len % 2 != 0) { return false; }
    return true;
}

static ByteVector combine(StringVector shares) {
    vector<ByteVector*> shares_bytes;
    size_t bytes_in_each_share = 0;
    for(auto share: shares) {
        auto cbor_buf = ur::Bytewords::decode(ur::Bytewords::standard, share);
        auto pos = cbor_buf.begin();
        auto end = cbor_buf.end();

        ur::CborLite::Tag tag;
        size_t value;
        ur::CborLite::decodeTagAndValue(pos, end, tag, value, ur::CborLite::Flag::requireMinimalEncoding);
        if(tag != ur::CborLite::Major::semantic || value != 309) {
            throw runtime_error("Not a valid SSKR share.");
        }
        ByteVector* bytes_buf = new ByteVector;
        ur::CborLite::decodeBytes(pos, end, *bytes_buf, ur::CborLite::Flag::requireMinimalEncoding);
        auto bytes_in_share = bytes_buf->size();
        if(bytes_in_each_share == 0) {
            bytes_in_each_share = bytes_in_share;
        } else if(bytes_in_share != bytes_in_each_share) {
            throw runtime_error("SSKR shares do not all have equal numbers of words.");
        }
        shares_bytes.push_back(bytes_buf);
    }

    vector<const uint8_t*> shares_bytes_pointers;
    for(const auto v: shares_bytes) {
        auto p = v->data();
        shares_bytes_pointers.push_back(p);
    }

    size_t result_len = bytes_in_each_share - METADATA_LENGTH_BYTES;
    ByteVector result(result_len) ;

    auto recovered_secret_len = sskr_combine(
        shares_bytes_pointers.data(), bytes_in_each_share, shares_bytes_pointers.size(),
        result.data(), result.size()
        );

    if(recovered_secret_len != result_len) {
        throw runtime_error("Invalid SSKR shares.");
    }

    for(auto p: shares_bytes) {
        delete p;
    }

    return result;
}

void FormatSSKR::process_input(Params* p) {
    if(p->is_ur_in) {
        for(auto& ur: p->ur_shares) {
            auto bw = ur::Bytewords::encode(ur::Bytewords::standard, ur.cbor());
            p->shares.push_back(bw);
        }
    } else {
        p->shares = p->get_multiple_arguments();
    }
    p->seed = combine(p->shares);
}

void FormatSSKR::process_output(Params* p) {
    // If we received a UR in and the input method was also SSKR, the actual
    // contained shares have been extracted in process_input() above, so we simply
    // output them.
    if(p->is_ur_in && !p->shares.empty()) {
        p->output = join(p->shares, "\n");
        return;
    }

    auto seed_len = p->seed.size();
    auto share_len = seed_len + METADATA_LENGTH_BYTES;
    auto share_count = sskr_count_shards(groups_threshold, groups.data(), groups.size());
    auto shares_buffer_len = share_len * share_count;

    if(!is_seed_length_valid(seed_len)) {
        throw runtime_error("Invalid seed length for SSKR. Must be in [16-32] and even.");
    }

    size_t bytes_in_each_share = 0;
    uint8_t shares_buffer[shares_buffer_len];

    int gen_share_count = sskr_generate(
        groups_threshold, groups.data(), groups.size(),
        p->seed.data(), seed_len,
        &bytes_in_each_share, shares_buffer, shares_buffer_len,
        NULL,
        p->rng
    );

    assert(gen_share_count == share_count);
    assert(bytes_in_each_share == share_len);

    if(p->is_ur_out) {
        StringVector strings;
        for (int i = 0; i < share_count; i++) {
            uint8_t* bytes = shares_buffer + (i * bytes_in_each_share);
            auto v = ByteVector(bytes, bytes + bytes_in_each_share);
            ByteVector cbor;
            ur::CborLite::encodeTagAndValue(cbor, ur::CborLite::Major::semantic, size_t(309));
            ur::CborLite::encodeBytes(cbor, v);
            ur::UR ur("crypto-sskr", cbor);
            auto s = ur::UREncoder::encode(ur);
            strings.push_back(s);
        }
        auto all_strings = join(strings, "\n");
        p->output = all_strings;
    } else {
        StringVector strings;
        for (int i = 0; i < share_count; i++) {
            uint8_t* bytes = shares_buffer + (i * bytes_in_each_share);
            auto v = ByteVector(bytes, bytes + bytes_in_each_share);
            ByteVector cbor;
            ur::CborLite::encodeTagAndValue(cbor, ur::CborLite::Major::semantic, size_t(309));
            ur::CborLite::encodeBytes(cbor, v);
            auto s = ur::Bytewords::encode(ur::Bytewords::standard, cbor);
            strings.push_back(s);
        }
        auto all_strings = join(strings, "\n");
        p->output = all_strings;
    }
}

FormatSSKR::FormatSSKR() : Format(Format::Key::sskr, "sskr") {
    groups_threshold = 1;
    groups = {{1, 1}};
}
