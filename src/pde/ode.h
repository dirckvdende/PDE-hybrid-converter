/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "grid/generator.h"
#include "grid/grid.h"
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
     * @param system A constant reference to the input PDE
     */
    ODEGenerator(const PDESystem &system);

    /**
     * Destructor
     */
    ~ODEGenerator();

    /**
     * Apply global compiler settings
     * @param settings The (updated) settings
     */
    void apply(const CompilerSettings &settings);

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
    bool hasTimeReference(const grid::Grid &grid) const;

    /**
     * Add a system that defines time variable "t"
     */
    void addTimeSystem();

    /**
     * Generate the initial iteration, which calculates initial conditions for
     * cells that are needed between iterations
     */
    void generateInitIteration();

    // The user input PDE system
    const PDESystem &pde;
    // Global compiler settings
    CompilerSettings settings;
    // The generated ODEs
    std::vector<ode::ODESystem> systems;
    // Object used to generate grid and expressions from PDE specification
    grid::GridGenerator gen;
    // Indicates if a time reference has been added already
    bool foundTime;

};

}