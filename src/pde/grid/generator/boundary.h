
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
     * Inherit constructor
     */
    using ExprGenerator::ExprGenerator;

    /**
     * Destructor
     */
    virtual ~BoundaryExprGenerator();

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

};

}