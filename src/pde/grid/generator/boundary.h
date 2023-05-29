
#pragma once

#include "generator.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"
#include "pde/spec.h"

namespace pde::grid::generator {

/**
 * This object can generate expressions for boundary cells on the grid
 */
class BoundaryExprGenerator final : public ExprGenerator {

public:

    /**
     * Constructor
     */
    BoundaryExprGenerator();

    /**
     * Destructor
     */
    virtual ~BoundaryExprGenerator();

    /**
     * Set the system that the user entered. This function expands the parent
     * definition to also do preprocessing of the boundary expressions
     * @param sys The system to save in this generator
     */
    virtual void setSystem(const PDESystem &sys);

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

};

}