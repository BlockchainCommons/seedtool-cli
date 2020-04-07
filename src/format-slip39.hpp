//
//  format-slip39.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef FORMAT_SLIP39_HPP
#define FORMAT_SLIP39_HPP

#include "format.hpp"

#include <stdbool.h>
#include <stdlib.h>

#include <bc-slip39/bc-slip39.h>

bool format_slip39_is_seed_length_valid(size_t seed_len);

format* format_slip39_new();

void format_slip39_set_groups_threshold(format* f, size_t groups_threshold);
void format_slip39_set_groups(format* f, const group_descriptor* groups, size_t groups_count);

#endif /* FORMAT_SLIP39_HPP */
