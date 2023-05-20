
#include "util.h"
#include <string>
#include <vector>

using namespace pde::grid;

std::string pde::grid::gridToString(const std::vector<size_t> &pos) {
    std::string out;
    for (size_t p : pos)
        out.append("_" + std::to_string(p));
    return out;
}

std::string pde::grid::gridFuncToString(std::string func, const
std::vector<size_t> &pos) {
    return func + "_" + gridToString(pos);
}