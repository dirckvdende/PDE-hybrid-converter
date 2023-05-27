
#pragma once

#include "domain/domain.h"
#include "grid.h"
#include "pde/parser/spec.h"

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

private:

    /**
     * Generate the domain specification
     */
    void generateDomain();

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

};

}