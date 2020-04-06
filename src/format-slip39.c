//
//  format-slip39.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "format-slip39.h"

#include <stdlib.h>
#include <bc-slip39/bc-slip39.h>
#include "params.h"

typedef struct slip39_options_t {
    group_descriptor* groups;
    size_t groups_count;
    size_t groups_threshold;
} slip39_options;
