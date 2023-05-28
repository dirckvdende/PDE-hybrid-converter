
#pragma once

#include "generator.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"

namespace pde::grid::generator {

class InternalExprGenerator final : public ExprGenerator {

public:

    /**
     * Constructor
     */
    InternalExprGenerator();

    /**
     * Destructor
     */
    ~InternalExprGenerator();

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

};

}