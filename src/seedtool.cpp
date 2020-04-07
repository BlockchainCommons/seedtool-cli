//
//  seedtool.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <stdio.h>
#include "params.h"
#include "format.h"

int main( int argc, char *argv[] ) {
    auto p = params_parse(argc, argv);
    p->input_format->process_input(p->input_format, p);
    p->output_format->process_output(p->output_format, p);
    
    if(p->output == NULL) {
        printf("*** Invalid output.\n");
        exit(1);
    }
    printf("%s\n", p->output);

    delete p;

    return 0;
}
