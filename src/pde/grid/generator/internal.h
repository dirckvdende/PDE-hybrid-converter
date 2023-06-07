
#pragma once

#include "pde/approx/approx.h"
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
     * Constructor
     * @param grid A reference to the grid that the expressions are to be
     * generated on
     * @param system A constant reference to the input PDE
     */
    InternalExprGenerator(Grid &grid, const PDESystem &system);

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
     * Replace raw variables (no derivatives) in the given grid cell
     * @param cell A reference to the grid cell
     */
    void replaceRawVars(GridCell &cell);

    /**
     * Calculate initial values for the given cell
     * @param cell A reference to the grid cell
     */
    void calcInit(GridCell &cell);

    // Approximation helper
    approx::SpatialApprox approx;

};

}