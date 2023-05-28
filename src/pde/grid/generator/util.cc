
#include "util.h"
#include <string>
#include <vector>

using namespace pde;
using namespace pde::grid;
using namespace pde::grid::generator;

std::string pde::grid::generator::toGridVar(const std::string &name,
const std::vector<size_t> &loc) {
    std::string out = "__var_" + name;
    for (size_t v : loc)
        out.append("_" + std::to_string(v));
    return out;
}