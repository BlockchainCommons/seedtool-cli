//
//  format-slip39.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <bc-slip39/bc-slip39.h>
#include <vector>

#include "format.hpp"

class FormatSLIP39 : public Format {
   public:
    FormatSLIP39();

    virtual void process_input(Params* p) override;
    virtual void process_output(Params* p) override;

    static bool is_seed_length_valid(size_t seed_len);

    size_t groups_threshold;
    std::vector<group_descriptor> groups;
};
