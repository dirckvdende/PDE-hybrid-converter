
#pragma once

#include "expr/expr.h"
#include "gridpoint.h"
#include "hypergrid.h"
#include <vector>

/**
 * A grid that stores expressions
 */
class ExprGrid : public HyperGrid<ExprGridPoint> {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     */
    ExprGrid(std::vector<size_t> dims);

    /**
     * Destructor
     */
    ~ExprGrid();

    /**
     * Get a string of all (existing) expressions from this grid
     * @return A string representation of all expressions from this grid
     * (separated by newlines)
     */
    std::string str() const;

private:

    /**
     * Generate point expressions for all grid points. Assumes that the grid
     * point types have been generated
     */
    void generatePointExpr();

    /**
     * Generate a point expression for a specific point. Assumes that the grid
     * point types have been generated
     * @param index The index of the grid point
     */
    void generatePointExpr(size_t index);

    /**
     * Generate the border expression at a specific position
     * @param index The index of the node to generate a border expression at
     */
    void generateBorderExpr(size_t index);

    /**
     * Get an expression for referencing a specific grid point
     * @param pos The position of the grid point
     * @return A constant reference to the expression
     */
    const ExprNode &getPointExpr(std::vector<size_t> pos) const;

    /**
     * Get the approximation of a certain derivative at a certain position
     * @param deriv The derivative count in each dimension
     * @param pos The position of the approximation (center)
     * @return The expression for the derivative approximation
     */
    ExprNode getApprox(std::vector<size_t> deriv, std::vector<size_t> pos) const;

};