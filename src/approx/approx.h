
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

    /**
     * Get the coefficients of an approximation of a derivative in one variable
     * @param n The number of times the derivative is taken
     * @param scale The scale of the grid cells that the approximation is taken
     * over
     */
    static std::vector<double> getCoeffs(size_t n, double scale);

    // Names of the spatial dimensions
    const std::vector<std::string> dims;
    // Map for dimension name indices
    std::unordered_map<std::string, size_t> dimMap;

};