
#pragma once

#include "expr/expr.h"
#include "generator.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"
#include "pde/spec.h"

namespace pde::grid::generator {

/**
 * This object can generate expressions for internal domain cells on the grid
 */
class InternalExprGenerator final : public ExprGenerator {

public:

    /**
     * Inherit constructor
     */
    using ExprGenerator::ExprGenerator;

    /**
     * Destructor
     */
    virtual ~InternalExprGenerator();

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

    /**
     * Replace all derivatives in a grid cell with approximations
     * @param cell A reference to the grid cell
     */
    void replaceAll(GridCell &cell);

    /**
     * Generate an approximation of a derivative at a given grid cell
     * @param cell A reference to the grid cell
     * @param deriv The derivative count per dimension
     * @param name The variable name to take the derivative of
     * @return The approximation as an expression
     */
    expr::ExprNode generateApprox(GridCell &cell, const std::vector<size_t>
    &deriv, const std::string &name);

    /**
     * Generate the dimension map, if it hasn't been already
     */
    void genDimMap();

    // Map from dimension names to indices, for quick access
    std::unordered_map<std::string, size_t> dimMap;

};

}