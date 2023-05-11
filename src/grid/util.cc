
#include "util.h"
#include <string>
#include <vector>

std::string gridToString(const std::vector<size_t> &pos) {
    std::string out;
    for (size_t p : pos)
        out.append("_" + std::to_string(p));
    return out;
}

std::string gridFuncToString(std::string func, const std::vector<size_t> &pos) {
    return func + "_" + gridToString(pos);
}