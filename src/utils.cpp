//
//  utils.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "utils.hpp"

#include <math.h>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include <bc-crypto-base/bc-crypto-base.h>

using namespace std;

const string data_to_hex(const byte_vector& in) {
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

const byte_vector hex_to_data(const string& hex) {
    byte_vector result;

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

const byte_vector data_to_base(const byte_vector& buf, size_t base) {
    byte_vector result;
    result.reserve(buf.size());
    for(auto b: buf) {
        result.push_back(roundf(b / 255.0 * (base - 1)));
    }
    return result;
}

const string data_to_alphabet(const byte_vector &in,
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

const string data_to_ints(const byte_vector &in,
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

const byte_vector digits_to_data(const string& in, size_t low, size_t high) {
    byte_vector result;
    for(auto c: in) {
        int n = c - '0';
        if(n < low || n > high) {
            throw runtime_error("Invalid digit.");
        }
        result.push_back(n);
    }
    return result;
}

const string join(const string_vector &strings, const string &separator) {
    ostringstream result;
    bool first = true;
    for(auto s: strings) {
        if(!first) {
            result << separator;
        }
        result << s;
        first = false;
    }
    return result.str();
}

const string_vector split(const string& s, const char& separator) {
	string_vector result;
	string buf;

	for(auto c: s) {
		if(c != separator) {
            buf += c;
        } else if(c == separator && buf.length() > 0) {
            result.push_back(buf);
            buf = "";
        }
	}

	if(buf != "") {
        result.push_back(buf);
    }

	return result;
}

const byte_vector sha256(const byte_vector &buf) {
    uint8_t digest[SHA256_DIGEST_LENGTH];
    sha256_Raw(&buf[0], buf.size(), digest);
    return byte_vector(digest, digest + SHA256_DIGEST_LENGTH);
}

const byte_vector crc32(const byte_vector &buf) {
    uint32_t checksum = crc32n(&buf[0], buf.size());
    auto cbegin = (uint8_t*)&checksum;
    auto cend = cbegin + sizeof(uint32_t);
    return byte_vector(cbegin, cend);
}

const string to_lower(const string& s) {
    string out;
    transform(s.begin(), s.end(), back_inserter(out), ::tolower);
    return out;
}

const bool has_prefix(const string& s, const string& prefix) {
    if(s.length() < prefix.length()) return false;
    return string(s.begin(), s.begin() + prefix.length()) == prefix;
}

const string take(const string &s, size_t count) {
    auto first = s.begin();
    auto c = min(s.size(), count);
    auto last = first + c;
    return string(first, last);
}

const string drop(const string& s, size_t count) {
    if(count >= s.length()) { return ""; }
    return string(s.begin() + count, s.end());
}

const string_vector partition(const string& s, size_t size) {
    string_vector result;
    auto remaining = s;
    while(remaining.length() > 0) {
        result.push_back(take(remaining, size));
        remaining = drop(remaining, size);
    }
    return result;
}

int days_since_epoch() {
    using namespace std::chrono;
    auto today = system_clock::now();
    auto time_since = today.time_since_epoch();
    typedef duration<int, ratio<60*60*24>> days;
    return duration_cast<days>(time_since).count();
}
