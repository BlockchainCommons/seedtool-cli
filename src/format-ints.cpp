//
//  format-ints.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-ints.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "params.hpp"
#include "utils.hpp"
#include "random.hpp"

using namespace std;

FormatInts::FormatInts() : Format(Format::Key::ints, "ints") {
    low = 0;
    high = 9;
}

static ByteVector parse_ints(const string& input) {
    ByteVector result;

    istringstream iss(input);

    while(!iss.eof()) {
        string s;
        iss >> s;
        int i;
        if(!(stringstream(s) >> i)) {
            throw runtime_error("Invalid integer. Allowed: [0-255]");
        }
        if(!(0 <= i && i <= 255)) {
            throw runtime_error("Integer out of range. Allowed: [0-255]");
        }
        result.push_back(i);
    }

    return result;
}

void FormatInts::process_input(Params* p) {
    auto input = p->get_combined_arguments();
    auto entropy = parse_ints(input);
    p->seed = deterministic_random(entropy, p->count);
}

void FormatInts::process_output(Params* p) {
    p->output = data_to_ints(p->seed, low, high, " ");
}
