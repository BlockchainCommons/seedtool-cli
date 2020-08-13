//
//  cbor.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef CBOR_UTILS_HPP
#define CBOR_UTILS_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <bc-ur/bc-ur.hpp>
#include "utils.hpp"

const auto cborDecodingFlags = ur::CborLite::Flag::requireMinimalEncoding;

template <typename Buffer>
void encode_byte_string(Buffer& cbor, const byte_vector& bytes) {
    ur::CborLite::encodeBytes(cbor, bytes);
}

template <typename Iter>
void decode_byte_string(Iter& pos, Iter end, byte_vector& bytes) {
    ur::CborLite::decodeBytes(pos, end, bytes, cborDecodingFlags);
}

template <typename Buffer>
void encode_string_array(Buffer& cbor, const string_vector& strings) {
    ur::CborLite::encodeArraySize(cbor, strings.size());
    for(auto string: strings) {
        ur::CborLite::encodeText(cbor, string);
    }
}

template <typename Iter>
void decode_string_array(Iter& pos, Iter end, string_vector& strings) {
    size_t size;
    ur::CborLite::decodeArraySize(pos, end, size, cborDecodingFlags);
    for(auto i = 0; i < size; i++) {
        std::string s;
        ur::CborLite::decodeText(pos, end, s, cborDecodingFlags);
        strings.push_back(s);
    }
}

template <typename Buffer>
void encode_array_of_string_arrays(Buffer& cbor, const std::vector<string_vector>& array_of_string_arrays) {
    ur::CborLite::encodeArraySize(cbor, array_of_string_arrays.size());
    for(auto string_array: array_of_string_arrays) {
        encode_string_array(cbor, string_array);
    }
}

template <typename Iter>
void decode_array_of_string_arrays(Iter& pos, Iter end, std::vector<string_vector>& array_of_string_arrays) {
    size_t size;
    ur::CborLite::decodeArraySize(pos, end, size, cborDecodingFlags);
    for(auto i = 0; i < size; i++) {
        string_vector strings;
        decode_string_array(pos, end, strings);
        array_of_string_arrays.push_back(strings);
    }
}

template <typename Buffer>
void encode_dict_with_birthdate(Buffer& cbor, const byte_vector& embedded_cbor) {
    ur::CborLite::encodeMapSize(cbor, size_t(2));
    ur::CborLite::encodeInteger(cbor, 1);
    append(cbor, embedded_cbor);
    ur::CborLite::encodeInteger(cbor, 2);
    ur::CborLite::encodeTagAndValue(cbor, ur::CborLite::Major::semantic, size_t(100));
    ur::CborLite::encodeInteger(cbor, days_since_epoch());
}

template <typename Iter, typename Func>
void decode_dict_with_birthdate(Iter& pos, Iter end, Func f) {
    size_t map_len;
    auto len = ur::CborLite::decodeMapSize(pos, end, map_len, cborDecodingFlags);
    std::set<int> labels;
    for(auto i = 0; i < map_len; i++) {
        int label;
        ur::CborLite::decodeInteger(pos, end, label, cborDecodingFlags);
        if(labels.find(label) != labels.end()) {
            throw std::runtime_error("Duplicate label.");
        }
        labels.insert(label);
        int date;
        switch (label) {
            case 1:
                f(pos, end);
                break;
            case 2:
                ur::CborLite::Tag tag;
                size_t value;
                ur::CborLite::decodeTagAndValue(pos, end, tag, value, cborDecodingFlags);
                if(tag != ur::CborLite::Major::semantic || value != 100) {
                    throw std::runtime_error("Invalid date.");
                }
                ur::CborLite::decodeInteger(pos, end, date, cborDecodingFlags);
                break;
            default:
                throw std::runtime_error("Unknown label.");
                break;
        }
    }
    if(pos != end) {
        throw std::runtime_error("Additional unknown bytes at end.");
    }
}

#endif // CBOR_UTILS_HPP