
#pragma once

#include "expr/expr.h"
#include "pde/grid/cell.h"
#include "pde/grid/domain/util.h"
#include "pde/grid/grid.h"
#include <unordered_map>
#include <vector>

namespace pde::approx {

/**
 * Object used to approximate spatial derivatives
 */
class SpatialApprox {

public:

    /**
     * Constructor
     * @param grid A reference to the grid on which approximations are
     * calculated
     */
    SpatialApprox(grid::Grid &grid);

    /**
     * Destructor
     */
    ~SpatialApprox();

    /**
     * Convert all spatial derivatives in the given grid cell
     * @param cell A reference to the cell in which to convert all derivatives
     */
    void convert(grid::GridCell &cell);

private:

    /**
     * Get the coefficients of a one-dimensional approximation of an n-th
     * derivative
     * @param n The n-th derivative is approximated
     * @return The list of coefficients
     */
    std::vector<double> getCoeffs(size_t n);

    /**
     * Replace all derivatives in an expression node with approximations
     * @param cell The cell that the node originates from
     * @param node A reference to the root node
     */
    void replaceApprox(grid::GridCell &cell, expr::ExprNode &node);

    /**
     * Convert a derivative expression node to an approximation
     * @param cell The cell that the node originates from
     * @param node A constant reference to the derivative node to convert
     * @return The converted node
     */
    expr::ExprNode getApprox(grid::GridCell &cell, const expr::ExprNode &node);

    // Type for a list of coordinates and coefficients associates to these
    // coordiantes
    typedef std::vector<std::pair<std::vector<long>, double>> Coeffs;

    /**
     * Get the coefficients of all relative grid points, given the derivative
     * counts
     * @param deriv The derivative count per dimension
     * @return A constant reference to a list of relative grid points and their
     * coefficients
     */
    const Coeffs &calcAllCoeffs(const std::vector<size_t> &deriv);

    /**
     * Get an expression for a position and coefficient
     * @param pos The position
     * @param coeff The coefficient multiplier
     * @param var The variable name that is approximated
     * @return The generated expression
     */
    expr::ExprNode getCoeffExpr(const std::vector<size_t> &pos, double coeff,
    const std::string &var) const;

    // Map from dimension names to dimension indices
    std::unordered_map<std::string, size_t> dimMap;
    // The grid on which all of the cells are defined
    grid::Grid &grid;
    // A cache to store derivative to coefficient conversions
    std::unordered_map<std::vector<size_t>, Coeffs, grid::domain::VectorHash>
    coeffCache;

};

}