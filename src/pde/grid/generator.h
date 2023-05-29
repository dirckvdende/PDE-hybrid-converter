
#pragma once

#include "domain/domain.h"
#include "generator/boundary.h"
#include "generator/internal.h"
#include "grid.h"
#include "groups/depgrid.h"
#include "pde/spec.h"

namespace pde::grid {

/**
 * This object is used to generate a grid and its expressions from a PDE
 * specification
 */
class GridGenerator {

public:

    /**
     * Constructor
     */
    GridGenerator();

    /**
     * Destructor
     */
    ~GridGenerator();

    /**
     * Set the input system
     * @param sys A constant reference to the input system of PDEs
     */
    void setSystem(const PDESystem &sys);

    /**
     * Run the generator
     */
    void run();

private:

    // The grid that operations are carried out on
    Grid grid;
    // User-entered system of PDEs
    PDESystem system;
    // Object used for generating the domain/boundary cells
    domain::Domain domain;
    // Object used for dividing the grid into groups and checking for
    // dependencies
    groups::DependGrid dependGrid;
    // Object used to determine the grid expressions on the boundary
    generator::BoundaryExprGenerator boundaryGen;
    // Object used to determine the grid expressions for internal cells (domain
    // cells)
    generator::InternalExprGenerator internalGen;

};

}