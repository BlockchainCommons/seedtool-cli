//
//  seedtool.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <iostream>
#include "params.hpp"
#include "format.hpp"

int main( int argc, char *argv[] ) {
    auto p = Params::parse(argc, argv);
    p->input_format->process_input(p);
    p->output_format->process_output(p);
    
    if(p->output.length() == 0) {
        printf("*** Invalid output.\n");
        exit(1);
    }
    std::cout << p->output << std::endl;

    delete p;

    return 0;
}
