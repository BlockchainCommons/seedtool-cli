//
//  format-ints.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatInts : public Format {
   public:
    FormatInts();

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);

    size_t low;
    size_t high;
};
