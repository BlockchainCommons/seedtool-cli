//
//  seedtool.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <stdio.h>
#include "generators.h"
#include "params.h"

int main( int argc, char *argv[] ) {
    params* p = parse_params(argc, argv);
    p->process_input(p);
    p->process_output(p);

    // char* string = random_ints(0, 9, 32, " ");
    // char* string = random_ints(1, 100, 10, " ");
    // char* string = random_ints(1, 6, 32, "");
    // char* string = random_ints(0, 5, 32, "");
    // char* string = random_ints(0, 9, 32, "");
    // char* string = random_cards(32);
    // char* string = random_cards(10);
    // char* string = random_hex(32);
    // char* string = random_binary(32);
    // char* string = random_bip39(32);

    // uint8_t group_threshold = 1;
    // group_descriptor groups[] = {
    //     {2, 3, NULL}
    //     // {3, 5, NULL},
    //     // {3, 5, NULL}
    // };
    // uint8_t group_count = 1;
    // char* string = random_slip39(16, group_threshold, groups, group_count);
    
    if(p->output == NULL) {
        printf("*** Invalid output.\n");
        exit(1);
    }
    printf("%s\n", p->output);

    dispose_params(p);

    return 0;
}
