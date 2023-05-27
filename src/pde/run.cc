
#include "run.h"
#include "compiler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace pde;

int pde::run(int argc, char *argv[]) {
    std::string filename;
    Compiler compiler;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.size() >= 1 && arg.front() != '-')
            filename = argv[i];
    }
    if (filename == "") {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::ifstream file(filename);
    std::stringstream stream;
    stream << file.rdbuf();
    compiler.setInput(stream.str());
    compiler.run();
    return 0;
}