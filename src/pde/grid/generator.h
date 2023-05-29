
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

    /**
     * Configure the grid before running calculations
     */
    void configureGrid();

    /**
     * Generate the domain specification
     */
    void generateDomain();

    /**
     * Generate the names for all of the grid cell and variable combinations
     */
    void generateNames();

    /**
     * Determine the dependency spread in each dimension
     */
    void calcSpread();

    /**
     * Divide the grid up into groups, using the set component limit. Also
     * determines if a cell value needs to be stored between iterations (if it
     * is in the domain anyway)
     */
    void divideGroups();

    /**
     * Get a string representation of which cells are contained in the domain
     * and which are not. Only works for 2D grids
     * @return The string representation
     */
    std::string domainStr() const;

    // The grid that operations are carried out on
    Grid grid;
    // User-entered system of PDEs
    PDESystem system;
    // Object used for generating the domain/boundary cells
    domain::Domain domain;
    // Object used for dividing the grid into groups and checking for
    // dependencies
    groups::DependGrid depends;
    // Object used to determine the grid expressions on the boundary
    generator::BoundaryExprGenerator boundaryGen;
    // Object used to determine the grid expressions for internal cells (domain
    // cells)
    generator::InternalExprGenerator internalGen;
    // Calculated dependency spread
    std::vector<size_t> spread;

};

}