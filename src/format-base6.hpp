//
//  format-base6.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatBase6 : public Format {
   public:
    FormatBase6() : Format(Format::Key::base6, "base6") {}

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);
};
