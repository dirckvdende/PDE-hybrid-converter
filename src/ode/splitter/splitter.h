
#pragma once

#include "ode/spec.h"
#include <vector>

namespace ode::splitter {

/**
 * Main object to split up systems of ODEs
 */
class Splitter {

public:

    /**
     * Constructor
     */
    Splitter();

    /**
     * Destructor
     */
    ~Splitter();

    /**
     * Set the list of input systems to split
     * @param val The list of input systems
     */
    void setSystems(const std::vector<ODESystem> &val);

    /**
     * Run the splitter
     */
    void run();

    /**
     * Get the list of output systems (splitted input), assumes run() has been
     * called
     * @return A constant reference to the list of output systems
     */
    const std::vector<ODESystem> &getOutputSystems() const;

private:

    /**
     * Split a single system, and append result to output systems list
     * @param system The system to split
     */
    void splitSystem(const ODESystem &system);

    // List of input systems
    std::vector<ODESystem> systems;
    // List of output systems
    std::vector<ODESystem> outputSystems;

};

}