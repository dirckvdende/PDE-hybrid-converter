
#pragma once

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
     * Generate an approximation of a derivative at a given grid cell. This
     * derivative will then be replaced in the given grid cell
     * @param cell A reference to the grid cell
     * @param deriv The derivative count per dimension
     */
    void generateApprox(GridCell &cell, const std::vector<size_t> &deriv);

    // Stores all of the spatial derivatives present in the expressions
    std::vector<std::vector<std::vector<size_t>>> derivs;

};

}