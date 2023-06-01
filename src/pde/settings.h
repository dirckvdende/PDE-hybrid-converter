
#pragma once

#include <vector>

namespace pde {

/**
 * Extra global compiler settings
 */
struct CompilerSettings {
    size_t componentLimit = 168;
    size_t maxGridSize = 1000000;
    std::string filename;
    std::string outputFilename = "tmp/compiler.ode";
};

}