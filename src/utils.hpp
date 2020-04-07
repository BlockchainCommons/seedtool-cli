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
