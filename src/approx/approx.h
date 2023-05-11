
#pragma once

#include "expr/expr.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Object used to create approxmations for spatial derivatives
 */
class SpatialApprox {

public:

    /**
     * Constructor
     * @param dims Names of the spatial dimensions
     */
    SpatialApprox(const std::vector<std::string> &dims);

    /**
     * Convert a spatial derivative at a grid point to an approximation
     * @param func The function that the derivative is taken over
     * @param pos The position on the grid
     * @param deriv List of derivatives taken
     * @return An expression, that is the approximation
     */
    ExprNode getApprox(std::string func, const std::vector<size_t> &pos,
    const std::vector<std::string> &deriv);

private:

    // Names of the spatial dimensions
    const std::vector<std::string> dims;
    // Map for dimension name indices
    std::unordered_map<std::string, size_t> dimMap;

};