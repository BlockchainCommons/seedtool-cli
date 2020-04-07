//
//  format-ints.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef FORMAT_INTS_HPP
#define FORMAT_INTS_HPP

#include "format.hpp"
#include <stdlib.h>

format* format_ints_new();

size_t format_ints_get_low(const format* f);
size_t format_ints_get_high(const format* f);

void format_ints_set_low(format* f, size_t low);
void format_ints_set_high(format* f, size_t high);

#endif /* FORMAT_INTS_HPP */
