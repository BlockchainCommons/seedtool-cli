//
//  utils.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "utils.hpp"

#include <math.h>
#include <stdexcept>

#include <bc-crypto-base/bc-crypto-base.h>

using namespace std;

string data_to_hex(const vector<uint8_t>& in) {
    auto hex = "0123456789abcdef";
    string result;
    for(auto c: in) {
        result.append(1, hex[(c >> 4) & 0xF]);
        result.append(1, hex[c & 0xF]);
    }
    return result;
}

uint8_t hex_digit_to_bin(char hex) {
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    } else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    } else {
        throw runtime_error("Invalid hex digit");
    }
}

vector<uint8_t> hex_to_data(const string& hex) {
    vector<uint8_t> result;

    auto len = hex.length();
    if(len % 2 != 0) {
        throw runtime_error("Hex string must have even number of characters.");
    }
    auto count = len / 2;
    result.reserve(count);
    for(auto i = 0; i < count; i++) {
        auto b1 = hex_digit_to_bin(hex[i * 2]);
        auto b2 = hex_digit_to_bin(hex[i * 2 + 1]);
        result.push_back((b1 << 4) | b2);
    }

    return result;
}

vector<uint8_t> data_to_base(const vector<uint8_t>& buf, size_t base) {
    vector<uint8_t> result;
    result.reserve(buf.size());
    for(auto b: buf) {
        result.push_back(roundf(b / 255.0 * (base - 1)));
    }
    return result;
}

string data_to_alphabet(const vector<uint8_t> &in, 
    size_t base, 
    string (to_alphabet)(size_t))
{
    string result;

    auto data = data_to_base(in, base);
    for(auto b: data) {
        auto s = to_alphabet(b);
        result.append(s);
    }

    return result;
}

string data_to_ints(const vector<uint8_t> &in,
    size_t low, size_t high, const string &separator)
{
    if (!(0 <= low && low < high && high <= 255)) {
        throw runtime_error("Int conversion range must be in 0 <= low < high <= 255.");
    }
    size_t base = high - low + 1;
    auto data = data_to_base(in, base);
    auto count = in.size();
    string result;
    for(auto i = 0; i < count; i++) {
        if (i > 0) {
            result += separator;
        }

        result += to_string(data[i] + low);
    }

    return result;
}

vector<uint8_t> digits_to_data(const string& in, size_t low, size_t high) {
    vector<uint8_t> result;
    for(auto c: in) {
        int n = c - '0';
        if(n < low || n > high) {
            throw runtime_error("Invalid digit.");
        }
        result.push_back(n);
    }
    return result;
}

string join(const vector<string> &strings, const string &separator) {
    string result;
    for(auto s: strings) {
        if(!result.empty()) {
            result += separator;
        }
        result += s;
    }
    return result;
}

vector<uint8_t> sha256(const vector<uint8_t> &buf) {
    uint8_t digest[SHA256_DIGEST_LENGTH];
    sha256_Raw(&buf[0], buf.size(), digest);
    return vector<uint8_t>(digest, digest + SHA256_DIGEST_LENGTH);
}
