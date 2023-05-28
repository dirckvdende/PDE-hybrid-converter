
#pragma once

#include "pde/grid/cell.h"
#include "pde/grid/grid.h"

namespace pde::grid::generator {

/**
 * Abstract base class for a grid expression generator
 */
class ExprGenerator {

public:

    /**
     * Constructor
     */
    ExprGenerator();

    /**
     * Destructor
     */
    ~ExprGenerator();

    /**
     * Set the grid that the generator should use
     * @param gridRef A reference to the grid to use for getting and output data
     */
    void setGrid(Grid &gridRef);

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell) = 0;

protected:

    // Grid that the generator should be getting data from and outputting data
    // to
    Grid *grid;

};

}