//
//  format-bch32.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatBC32 : public Format {
   public:
    FormatBC32() : Format(Format::Key::bc32, "bc32") {}

    virtual void process_input(Params* p) override;
    virtual void process_output(Params* p) override;

    static bool is_seed_length_valid(size_t seed_len);
};
