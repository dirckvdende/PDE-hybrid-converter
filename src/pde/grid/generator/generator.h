
#pragma once

#include "pde/grid/cell.h"
#include "pde/grid/grid.h"
#include "pde/parser/spec.h"

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
     * Set the system that the user entered
     * @param sys The system to save in this generator
     */
    void setSystem(const parser::PDESystem &sys);

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell) = 0;

protected:

    // The PDE system to use for generating expressions
    parser::PDESystem system;
    // Grid that the generator should be getting data from and outputting data
    // to
    Grid *grid;

};

}