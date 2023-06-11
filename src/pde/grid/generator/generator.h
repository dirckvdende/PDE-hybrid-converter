/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "pde/grid/cell.h"
#include "pde/grid/grid.h"
#include "pde/spec.h"

namespace pde::grid::generator {

/**
 * Abstract base class for a grid expression generator
 */
class ExprGenerator {

public:

    /**
     * Constructor
     * @param grid A reference to the grid that the expressions are to be
     * generated on
     * @param system A constant reference to the input PDE
     */
    ExprGenerator(Grid &grid, const PDESystem &system);

    /**
     * Destructor
     */
    virtual ~ExprGenerator();

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell) = 0;

protected:

    // The PDE system to use for generating expressions
    const PDESystem &system;
    // Grid that the generator should be getting data from and outputting data
    // to
    Grid &grid;

};

}