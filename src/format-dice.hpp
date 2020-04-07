//
//  format-dice.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatDice : public Format {
   public:
    FormatDice() : Format(Format::Key::dice, "dice") {}

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);
};
