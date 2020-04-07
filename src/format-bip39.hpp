//
//  format-bip39.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "format.hpp"

class FormatBIP39 : public Format {
   public:
    FormatBIP39() : Format(Format::Key::bip39, "bip39") {}

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);

    static bool is_seed_length_valid(size_t seed_len);
};
