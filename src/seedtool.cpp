//
//  seedtool.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <iostream>
#include <stdexcept>

#include "params.hpp"
#include "format.hpp"

#include "cbor-lite/codec.h"

using namespace std;

int main( int argc, char *argv[] ) {
    auto p = Params::parse(argc, argv);

    try {
        p->input_format->process_input(p);
        p->output_format->process_output(p);
        
        if(p->output.empty()) {
            throw runtime_error("An internal error occurred.");
        }

        cout << p->output << endl;

    } catch(exception &e) {
        cerr << argv[0] << ": " << e.what() << endl;
        exit(1);
    }

    delete p;
    return 0;
}
