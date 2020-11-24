//
//  utils.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <vector>

typedef std::vector<uint8_t> ByteVector;
typedef std::vector<std::string> StringVector;

const std::string data_to_hex(const ByteVector& in);
uint8_t hex_digit_to_bin(char hex);

const ByteVector hex_to_data(const std::string& hex);

const ByteVector data_to_base(const ByteVector& buf, size_t base);

const std::string data_to_alphabet(const ByteVector &in,
    size_t base,
    std::string (to_alphabet)(size_t));

const std::string data_to_ints(const ByteVector &in,
    size_t low, size_t high, const std::string &separator);

const ByteVector digits_to_data(const std::string& in, size_t low, size_t high);

const std::string join(const StringVector &strings, const std::string &separator);
const StringVector split(const std::string& s, const char& separator);

const ByteVector sha256(const ByteVector &buf);

template<typename T>
const std::vector<T> take(const std::vector<T> &buf, size_t count) {
    auto first = buf.begin();
    auto c = std::min(buf.size(), count);
    auto last = first + c;
    return std::vector(first, last);
}

const std::string take(const std::string &s, size_t count);

template<typename T>
void append(std::vector<T>& target, const std::vector<T>& source) {
    target.insert(target.end(), source.begin(), source.end());
}

int days_since_epoch();

template<typename T>
const std::vector<T> flatten(const std::vector<std::vector<T>>& source) {
    std::vector<T> result;
    for(auto item: source) {
        append(result, item);
    }
    return result;
}
