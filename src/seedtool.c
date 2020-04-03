//
//  seedtool.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <argp.h>
#include <stdio.h>

char doc[] = "A program with a help and default options";
struct argp argp = { NULL, NULL, NULL, doc };
int main( int argc, char *argv[] ) {
    argp_parse( &argp, argc, argv, 0, 0, NULL );
    return 0;
}
