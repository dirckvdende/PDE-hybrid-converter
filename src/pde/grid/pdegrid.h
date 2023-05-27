
#pragma once

#include "domain/domain.h"
#include "grid.h"
#include "pde/parser/spec.h"
#include "groups/depgrid.h"

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

private:

    /**
     * Generate the domain specification
     */
    void generateDomain();

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
     * Generate border cell expressions
     */
    void generateBorderExpressions();

    /**
     * Generate a border expression for a specific cell
     * @param cell A reference to the cell to generate the border expression for
     */
    void generateBorderExpression(GridCell &cell);

    /**
     * Generate domain cell expressions
     */
    void generateDomainExpressions();

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

};

}