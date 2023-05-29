
#pragma once

#include "expr/expr.h"
#include "pde/grid/cell.h"
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
     */
    SpatialApprox();

    /**
     * Destructor
     */
    ~SpatialApprox();

    /**
     * Store the scale of the grid in this object
     * @param val The scale of the grid
     */
    void setScale(double val);

    /**
     * Set the names of the dimensions
     * @param dims A list of the names of the dimensions, in order
     */
    void setDims(const std::vector<std::string> &dims);

    /**
     * Set the grid on which all of the cells to be processed are present
     * @param gridRef A reference to the grid
     */
    void setGrid(grid::Grid &gridRef);

    /**
     * Set the current iteration
     * @param val The new value for the current iteration
     */
    void setIteration(size_t val);

    /**
     * Convert all spatial derivatives in the stored grid
     */
    void convert();

private:

    /**
     * Convert all spatial derivatives in the given grid cell
     * @param cell A reference to the cell in which to convert all derivatives
     */
    void convert(grid::GridCell &cell);

    /**
     * Get the coefficients of a one-dimensional approximation of an n-th
     * derivative
     * @param n The n-th derivative is approximated
     * @return The list of coefficients
     */
    std::vector<double> getCoeffs(size_t n);

    /**
     * Replace all derivatives in an expression node with approximations
     * @param node A reference to the root node
     */
    void replaceApprox(expr::ExprNode &node) const;

    /**
     * Convert a derivative expression node to an approximation
     * @param node A constant reference to the derivative node to convert
     * @return The converted node
     */
    expr::ExprNode getApprox(const expr::ExprNode &node) const;

    // Grid scale
    double scale;
    // Map from dimension names to dimension indices
    std::unordered_map<std::string, size_t> dimMap;
    // The grid on which all of the cells are defined
    grid::Grid *grid;
    // Keep track of the current cell being processed
    grid::GridCell *curCell;
    // Current iteration
    size_t iteration;

};

}