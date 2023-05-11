
#include "approx.h"

SpatialApprox::SpatialApprox(const std::vector<std::string> &dims) : dims(dims)
{
    for (size_t i = 0; i < dims.size(); i++)
        dimMap.emplace(dims[i], i);
}

ExprNode SpatialApprox::getApprox(std::string func, const std::vector<size_t>
&pos, const std::vector<std::string> &deriv) {
    std::vector<size_t> amounts(dims.size(), 0);
    for (const std::string &d : deriv)
        amounts[dimMap[d]]++;
    // TODO: implement
}