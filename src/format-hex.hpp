//
//  format-hex.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatHex : public Format {
   public:
    FormatHex() : Format(Format::Key::hex, "hex") {}

    virtual void process_input(Params* p) override;
    virtual void process_output(Params* p) override;
};
