//
//  main.cpp
//  
//
//  Created by Ian Squiers on 9/25/18.
//

#include <stdio.h>
#include <ga.cpp>
#include <pbil.cpp>

main(int argc, char* argv[]) {
    if (argv[8] == 'g') {
        ga(argv);
    }
    else {
        pbil(argv);
    }
    return 0;
}
