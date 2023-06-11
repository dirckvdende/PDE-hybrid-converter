/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

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
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);
    compiler.readArgs(args);
    if (!compiler.settingsValid()) {
        std::cerr << "Usage: " << argv[0] << " [options] <filename>" <<
        std::endl;
        return 1;
    }
    compiler.run();
    return 0;
}