//
//  ur.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <stdint.h>

#include <vector>
#include <string>

#include "utils.hpp"

std::vector<std::string> encode_ur(const byte_vector& cbor, const std::string& type, size_t max_part_len);

struct UR {
    std::string type;
    byte_vector cbor;

    UR(const std::vector<std::string>& encoded_parts);
};
