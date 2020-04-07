//
//  format-bits.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatBits : public Format {
   public:
    FormatBits() : Format(Format::Key::bits, "bits") {}

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);
};
