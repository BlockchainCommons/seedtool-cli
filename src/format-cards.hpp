//
//  format-cards.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format.hpp"

class FormatCards : public Format {
   public:
    FormatCards() : Format(Format::Key::cards, "cards") {}

    virtual void process_input(Params* p);
    virtual void process_output(Params* p);
};
