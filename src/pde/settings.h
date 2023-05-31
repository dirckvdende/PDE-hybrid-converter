
#pragma once

#include <vector>

namespace pde {

/**
 * Extra global compiler settings
 */
struct CompilerSettings {
    size_t maxGridSize = 100000;
    size_t componentLimit = 168;
};

}