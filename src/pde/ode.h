
#pragma once

#include "grid/pdegrid.h"
#include "ode/spec.h"
#include "parser/spec.h"

namespace pde {

/**
 * Object that generated ODE systems from a grid. It generates ODE systems for
 * all iterations
 */
class ODEGenerator {

public:

    /**
     * Constructor
     */
    ODEGenerator();

    /**
     * Destructor
     */
    ~ODEGenerator();

    /**
     * Set the grid that holds the information to be processed by the ODE
     * generator
     * @param gridRef A reference to the grid
     */
    void setGrid(grid::PDEGrid &gridRef);

    /**
     * Set the input PDE system
     * @param sys The PDE system
     */
    void setPDE(const parser::PDESystem &sys);

    /**
     * Run the generator and store generated ODE systems in this object
     */
    void run();

    /**
     * Get the generated ODE systems, which will be filled if run() was called
     * @return A constant reference to the list of ODE systems, which are in
     * order
     */
    const std::vector<ode::ODESystem> &getSystems() const;

private:

    /**
     * Check if the grid contains an expression that references time
     * @return A boolean indicating if a reference to "t" is present
     */
    bool hasTimeReference() const;

    /**
     * Add a system that defines time variable "t"
     */
    void addTimeSystem();

    // The user input PDE system
    parser::PDESystem pde;
    // A pointer to the grid that captures information about the final ODE
    // systems
    grid::PDEGrid *grid;
    // The generated ODEs
    std::vector<ode::ODESystem> systems;

};

}