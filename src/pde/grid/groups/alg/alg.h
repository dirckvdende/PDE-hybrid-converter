/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "pde/grid/groups/depgrid.h"

namespace pde::grid::groups::alg {

/**
 * Base class for testing division algorithms
 */
class DivisionAlg {

public:

    /**
     * Constructor
     * @param depGrid A reference to the dependency grid to use (default is
     * nullptr)
     */
    DivisionAlg();
    DivisionAlg(DependGrid &depGrid);

    /**
     * Destructor
     */
    virtual ~DivisionAlg();

    /**
     * Set the dependency grid that the algorithm should run on
     * @param depGrid A reference to the new dependency grid
     */
    void setGrid(DependGrid &depGrid);

    /**
     * Run the division algorithm
     */
    virtual void run() = 0;

protected:

    // The group grid used to create groups of cells
    DependGrid *grid;

};

}