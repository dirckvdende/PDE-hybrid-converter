
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
     * Run the generator general calculations before expression generation
     */
    void prepare();

    /**
     * Run the generator for a specific iteration
     * @param iteration The iteration number
     */
    void run(size_t iteration);

    // The grid that operations are carried out on
    Grid grid;

private:

    /**
     * Determine the dependency spread in each dimension
     */
    void calcSpread();

    /**
     * Generate the domain specification
     */
    void generateDomain();

    /**
     * Divide the grid up into groups, using the set component limit. Also
     * determines if a cell value needs to be stored between iterations (if it
     * is in the domain anyway)
     */
    void divideGroups();

    /**
     * Generate the names for all of the grid cell and variable combinations
     */
    void generateNames();

    /**
     * Generate expressions for all of the nodes in the grid (that are boundary
     * or domain)
     */
    void generateExpr();

    /**
     * Generate emit specifications for all nodes in the grid (that are boundary
     * or domain)
     */
    void generateEmits();

    /**
     * Convert a variable name with a given cell to a positioned variable, to be
     * used by the final emits of the system
     * @param var The variable name
     * @param cell The grid cell
     * @return The positioned variable name
     */
    std::string toPosVar(const std::string &var, const GridCell &cell) const;

    /**
     * Get a string representation of which cells are contained in the domain
     * and which are not. Only works for 2D grids
     * @return The string representation
     */
    std::string domainStr() const;

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

};

}