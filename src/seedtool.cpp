//
//  seedtool.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <stdio.h>
#include "params.hpp"
#include "format.hpp"

int main( int argc, char *argv[] ) {
    auto p = params_parse(argc, argv);
    p->input_format->process_input(p->input_format, p);
    p->output_format->process_output(p->output_format, p);
    
    if(p->output.length() == 0) {
        printf("*** Invalid output.\n");
        exit(1);
    }
    printf("%s\n", p->output.c_str());

    delete p;

    return 0;
}
