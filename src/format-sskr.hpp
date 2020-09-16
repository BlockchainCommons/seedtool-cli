//
//  format-sskr.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include <bc-sskr/bc-sskr.h>
#include <vector>

#include "format.hpp"
#include "utils.hpp"

class FormatSSKR : public Format {
   public:
    FormatSSKR();

    virtual void process_input(Params* p) override;
    virtual void process_output(Params* p) override;

    static bool is_seed_length_valid(size_t seed_len);

    size_t groups_threshold;
    std::vector<sskr_group_descriptor> groups;
};
