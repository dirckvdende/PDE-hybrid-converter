
#pragma once

#include "generator.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"

namespace pde::grid::generator {

/**
 * This object can generate expressions for internal domain cells on the grid
 */
class InternalExprGenerator final : public ExprGenerator {

public:

    /**
     * Constructor
     */
    InternalExprGenerator();

    /**
     * Destructor
     */
    virtual ~InternalExprGenerator();

    /**
     * Set the system that the user entered. This function expands the parent
     * definition to also do preprocessing of the boundary expressions
     * @param sys The system to save in this generator
     */
    virtual void setSystem(const parser::PDESystem &sys);

    /**
     * Generate the expression for a given grid cell
     * @param cell A reference to the grid cell
     */
    virtual void generate(GridCell &cell);

private:

    /**
     * Find all derivatives in all variable expressions and store them
     */
    void findAllDerivs();

    /**
     * Generate an approximation of a derivative at a given grid cell. This
     * derivative will then be replaced in the given grid cell
     * @param cell A reference to the grid cell
     * @param deriv The derivative count per dimension
     */
    void generateApprox(GridCell &cell, const std::vector<size_t> &deriv);

    // Stores all of the spatial derivatives present in the expressions
    std::vector<std::vector<std::vector<size_t>>> derivs;

};

}