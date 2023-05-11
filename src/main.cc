
#include "compiler/compiler.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Compiler compiler(argc, argv);
    compiler.run();
    return 0;
}
