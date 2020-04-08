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

std::string data_to_hex(const std::vector<uint8_t>& in);
uint8_t hex_digit_to_bin(char hex);

std::vector<uint8_t> hex_to_data(const std::string& hex);

std::vector<uint8_t> data_to_base(const std::vector<uint8_t>& buf, size_t base);

std::string data_to_alphabet(const std::vector<uint8_t> &in, 
    size_t base, 
    std::string (to_alphabet)(size_t));

std::string data_to_ints(const std::vector<uint8_t> &in,
    size_t low, size_t high, const std::string &separator);

std::vector<uint8_t> digits_to_data(const std::string& in, size_t low, size_t high);

std::string join(const std::vector<std::string> &strings, const std::string &separator);

std::vector<uint8_t> sha256(const std::vector<uint8_t> &buf);

template<typename T>
std::vector<T> take(const std::vector<T> &buf, size_t count) {
    auto first = buf.begin();
    auto c = std::min(buf.size(), count);
    auto last = first + c;
    return std::vector(first, last);
}
