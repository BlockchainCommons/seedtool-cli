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

typedef std::vector<uint8_t> byte_vector;
typedef std::vector<std::string> string_vector;

const std::string data_to_hex(const byte_vector& in);
uint8_t hex_digit_to_bin(char hex);

const byte_vector hex_to_data(const std::string& hex);

const byte_vector data_to_base(const byte_vector& buf, size_t base);

const std::string data_to_alphabet(const byte_vector &in,
    size_t base,
    std::string (to_alphabet)(size_t));

const std::string data_to_ints(const byte_vector &in,
    size_t low, size_t high, const std::string &separator);

const byte_vector digits_to_data(const std::string& in, size_t low, size_t high);

const std::string join(const string_vector &strings, const std::string &separator);
const string_vector split(const std::string& s, const char& separator);

const byte_vector sha256(const byte_vector &buf);
const byte_vector crc32(const byte_vector &buf);

const std::string to_lower(const std::string& s);
const bool has_prefix(const std::string& s, const std::string& prefix);

template<typename T>
const std::vector<T> take(const std::vector<T> &buf, size_t count) {
    auto first = buf.begin();
    auto c = std::min(buf.size(), count);
    auto last = first + c;
    return std::vector(first, last);
}

const std::string take(const std::string &s, size_t count);
const std::string drop(const std::string &s, size_t count);

template<typename T>
void append(std::vector<T>& target, const std::vector<T>& source) {
    target.insert(target.end(), source.begin(), source.end());
}

const string_vector partition(const std::string& string, size_t size);

int days_since_epoch();

template<typename T>
const std::vector<T> flatten(const std::vector<std::vector<T>>& source) {
    std::vector<T> result;
    for(auto item: source) {
        append(result, item);
    }
    return result;
}
