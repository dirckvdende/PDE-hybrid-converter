
#pragma once

#include "grid/pdegrid.h"
#include "ode/spec.h"
#include "settings.h"
#include "spec.h"

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
     * Set the global compiler settings
     * @param val The (updated) settings
     */
    void changeSettings(const CompilerSettings &val);

    /**
     * Set the input PDE system
     * @param sys The PDE system
     */
    void setPDE(const PDESystem &sys);

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
     * @param grid The grid to search through
     * @return A boolean indicating if a reference to "t" is present
     */
    bool hasTimeReference(const grid::PDEGrid &grid) const;

    /**
     * Add a system that defines time variable "t"
     */
    void addTimeSystem();

    // The user input PDE system
    PDESystem pde;
    // Global compiler settings
    CompilerSettings settings;
    // The generated ODEs
    std::vector<ode::ODESystem> systems;

};

}