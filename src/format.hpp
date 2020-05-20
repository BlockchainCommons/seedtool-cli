//
//  format.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <string>

class Params;

class Format {
   public:
    enum class Key {
        unknown = -1,
        random,
        hex,
        bits,
        cards,
        dice,
        base6,
        base10,
        ints,
        bip39,
        slip39,
        bc32
    };

    Format(Key key, const std::string &name) : key(key), name(name) {}
    virtual ~Format() {}

    Key key;
    std::string name;

    virtual void process_input(Params *) = 0;
    virtual void process_output(Params *) = 0;

    static Key key_for_string(const std::string &arg);
};
