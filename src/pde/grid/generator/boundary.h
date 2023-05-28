
#pragma once

#include "generator.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"

namespace pde::grid::generator {

class BoundaryExprGenerator final : public ExprGenerator {

public:

    /**
     * Constructor
     */
    BoundaryExprGenerator();

    /**
     * Destructor
     */
    ~BoundaryExprGenerator();

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

};

}