/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <string>
#include <vector>

namespace pde {

/**
 * Extra global compiler settings
 */
struct CompilerSettings {
    size_t componentLimit = 1000000;
    size_t maxGridSize = 1000000;
    std::string filename;
    std::string outputFilename = "tmp/compiler.ode";
    bool doSplitter = true;
};

}