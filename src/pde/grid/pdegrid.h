
#pragma once

#include "domain/domain.h"
#include "grid.h"
#include "pde/parser/spec.h"
#include "generator/boundary.h"
#include "generator/internal.h"
#include "groups/depgrid.h"
#include <string>

namespace pde::grid {

/**
 * A derived class of the basic grid that only carries basic information and
 * functions, this one is used by the compiler and has more functionality and
 * interacts with other components
 */
class PDEGrid : public Grid {

public:

    /**
     * Constructor
     */
    PDEGrid();

    /**
     * Destructor
     */
    virtual ~PDEGrid();

    /**
     * Generate all grid contents and auxilliary information
     */
    void generate();

    /**
     * Set the hardware component limit (maximum number of grid cells per group)
     * @param val New maximum
     */
    void setComponentLimit(size_t val);

    /**
     * Set the input PDE system
     * @param val The PDE system specification
     */
    void setSystem(const parser::PDESystem &val);

    /**
     * Set the maximum number of cells that the grid bounding box should contain
     * @param val The new maximum
     */
    void setMaxGridSize(size_t val);

private:

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
     * Generate all expressions on the domain and boundary
     */
    void generateExpressions();

    /**
     * Get a string representation of which cells are contained in the domain
     * and which are not. Only works for 2D grids
     * @return The string representation
     */
    std::string domainStr() const;

    // PDE user input specification
    parser::PDESystem system;
    // Object used for determining which cells are in the domain
    domain::Domain domain;
    // Hardware component limit (abstract)
    size_t componentLimit;
    // Object used for determining the groups that grid cells should be divided
    // into
    groups::DependGrid depends;
    // Dependency spread
    std::vector<size_t> spread;
    // Generators for expressions on the boundary and domain
    generator::BoundaryExprGenerator boundaryGen;
    generator::InternalExprGenerator internalGen;

};

}