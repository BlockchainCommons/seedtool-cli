//
//  format-base10.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatBase10 : public Format {
   public:
    FormatBase10() : Format(Format::Key::base10, "base10") {}

    virtual void process_input(Params* p) override;
    virtual void process_output(Params* p) override;
};
