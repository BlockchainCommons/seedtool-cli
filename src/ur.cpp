//
//  ur.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "ur.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <math.h>
#include <bc-ur/bc-ur.hpp>

using namespace std;

static size_t bytewords_encoded_length(size_t payload_len) {
    return size_t(payload_len + 4) * 2;
}

static string sequence_element(size_t seq_num, size_t seq_len) {
    ostringstream o;
    o << seq_num << "of" << seq_len;
    return o.str();
}

static size_t fragment_len(size_t encoded_payload_len, size_t seq_len) {
    return size_t(ceil(double(encoded_payload_len) / seq_len));
}

static size_t length_of_ur(size_t type_len, size_t fragment_len, size_t seq_num,
                           size_t seq_len, bool include_seq,
                           bool include_hash) {
    size_t scheme_len = 2; // ur

    auto len = scheme_len + 1; // ":"
    len += type_len + 1; // "/"
    if(include_seq) {
        len += sequence_element(seq_num, seq_len).length() + 1; // "/"
    }
    if(include_hash) {
        len += bytewords_encoded_length(4) + 1; // "/"
    }
    len += fragment_len;
    return len;
}

static size_t find_number_of_parts(size_t type_len, size_t payload_len, size_t max_part_len) {
    auto seq_len = 0;
    size_t encoded_payload_len = bytewords_encoded_length(payload_len);
    size_t ur_len;
    do {
        seq_len += 1;
        size_t frag_len = fragment_len(encoded_payload_len, seq_len);
        if(frag_len <= 10) {
            throw runtime_error("Maximum part length too small.");
        }
        auto seq_num = seq_len; // use maximum because it's got the longest length
        auto include_seq = seq_len > 1;
        auto include_hash = include_seq;
        ur_len = length_of_ur(type_len, frag_len, seq_num, seq_len, include_seq, include_hash);
    } while(ur_len > max_part_len);
    return seq_len;
}

static string encode_part(const string& fragment, const string& type,
                           size_t seq_num, size_t seq_len,
                           const string& hash)
{
    ostringstream o;
    o << "ur:";
    o << type << "/";
    if(seq_len > 1) {
        o << sequence_element(seq_num, seq_len) << "/";
        o << hash << "/";
    }
    o << fragment;
    return o.str();
}

string_vector encode_ur(const byte_vector& cbor, const string& type, size_t max_part_len) {
    auto seq_len = find_number_of_parts(type.length(), cbor.size(), max_part_len);

    auto payload = ur::Bytewords::encode(ur::Bytewords::style::minimal, cbor);
    auto payload_len = payload.length();
    auto frag_len = fragment_len(payload_len, seq_len);
    auto hash = ur::Bytewords::encode(ur::Bytewords::style::minimal, crc32(cbor));

    auto fragments = partition(payload, frag_len);
    assert(seq_len == fragments.size());

    auto seq_num = 1;
    string_vector parts;
    for(auto fragment: fragments) {
        auto part = encode_part(fragment, type, seq_num, seq_len, hash);
        parts.push_back(part);
        seq_num += 1;
    }

    return parts;
}

struct URPart {
    string type;
    size_t seq_num = 1;
    size_t seq_len = 1;
    string fragment;
    byte_vector hash;

    bool operator <(const URPart& rhs) const {
        return seq_num < rhs.seq_num;
    }

    bool is_type_char(char c) {
        if('a' <= c && c <= 'z') return true;
        if('0' <= c && c <= '9') return true;
        if(c == '-') return true;
        return false;
    }

    URPart(const string& encoded_part) {
        // Don't consider case.
        auto lower_part = to_lower(encoded_part);

        // Validate URI scheme.
        if(!has_prefix(lower_part, "ur:")) {
            throw runtime_error("UR: Invalid scheme.");
        }
        auto path = drop(lower_part, 3);

        // Split the remainder into path components
        auto components = split(path, '/');

        // Validate the number of path components
        auto components_len = components.size();
        // 0: invalid
        // 1: invalid
        // 2: type/fragment
        // 3: type/hash/fragment
        // 4: type/seq/hash/fragment
        // 5...: invalid
        if(components_len < 2) {
            throw runtime_error("UR: Too few path components");
        }
        if(components_len > 4) {
            throw runtime_error{"UR: Too many path components"};
        }

        // Validate the type component
        type = components.front();
        for(auto c: type) {
            if(!is_type_char(c)) {
                throw runtime_error{"UR: Invalid type component."};
            }
        }

        // The fragment is always the last path component.
        fragment = components.back();

        // If we only had type/fragment, we're done.
        if(components_len == 2) {
            return;
        }

        // Determine the position of the hash component
        size_t hash_index;
        if(components_len == 3) {
            hash_index = 1;
        } else {
            hash_index = 2;

            // Parse the sequence component.
            auto items = sscanf(components[1].c_str(), "%zdof%zd", &seq_num, &seq_len);
            if(items != 2) {
                throw runtime_error("UR: Invalid sequence component.");
            }
            if(seq_len < 1 || seq_num > seq_len) {
                throw runtime_error("UR: Invalid sequence component.");
            }
        }

        // Decode the hash.
        try {
            hash = ur::Bytewords::decode(ur::Bytewords::style::minimal, components[hash_index]);
        } catch(...) {
            throw runtime_error("UR: Invalid hash.");
        }
    }
};

UR::UR(const string_vector& encoded_parts) {
    if(encoded_parts.empty()) {
        throw runtime_error("UR: Incomplete sequence.");
    }

    // Decode all the parts
    vector<URPart> parts;
    for(auto encoded_part: encoded_parts) {
        parts.push_back(URPart(encoded_part));
    }

    // Validate the sequence length
    auto seq_len = parts.front().seq_len;
    if(parts.size() != seq_len) {
        throw runtime_error("UR: Incomplete sequence.");
    }

    // All parts' sequence lengths, types, and hashes must match
    type = parts.front().type;
    auto hash = parts.front().hash;
    if(seq_len > 1) {
        for(auto part: parts) {
            if(part.seq_len != seq_len) {
                throw runtime_error("UR: Invalid sequence.");
            }
            if(part.type != type) {
                throw runtime_error("UR: Invalid sequence.");
            }
            if(part.hash != hash) {
                throw runtime_error("UR: Invalid sequence.");
            }
        }
    }

    // Put them in order
    sort(parts.begin(), parts.end());

    // All parts must have monotonically ascending sequence numbers
    for(auto i = 0; i < seq_len; i++) {
        if(parts[i].seq_num != i + 1) {
            throw runtime_error("UR: Invalid sequence.");
        }
    }

    // Assemble the fragments
    string encoded_cbor;
    for(auto part: parts) {
        encoded_cbor += part.fragment;
    }

    // Decode the payload
    try {
        cbor = ur::Bytewords::decode(ur::Bytewords::style::minimal, encoded_cbor);
    } catch(...) {
        throw runtime_error("UR: Invalid payload.");
    }

    // Check the hash
    if(!hash.empty()) {
        auto cborChecksum = crc32(cbor);
        if(hash != cborChecksum) {
            throw runtime_error("UR: Hash didn't match.");
        }
    }

    // decode successful
}
