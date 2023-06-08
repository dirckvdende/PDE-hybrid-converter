
#pragma once

#include "pde/spec.h"
#include "presystem.h"
#include <string>

namespace pde::parser {

/**
 * Object used to convert PreSystem to a PDESystem specification object. Assumes
 * that all of the configurations are valid
 */
class SystemGen {

public:

    /**
     * Constructor
     * @param preSystem A constant reference to the presystem to generate the
     * PDE specification from
     */
    SystemGen(const PreSystem &preSystem);

    /**
     * Destructor
     */
    ~SystemGen();

    /**
     * Run the system generator
     */
    void run();

    /**
     * Get the generated PDE specidication
     * @return A constant reference to the generated PDE system
     */
    const PDESystem &getSystem() const;

private:

    /**
     * Find all variable names and process them
     */
    void findVarNames();

    // A constant reference to the presystem to generate the PDE spec from
    const PreSystem &preSystem;
    // The generated PDE specification
    PDESystem system;
    // Map from variable names to indices
    std::unordered_map<std::string, size_t> varMap;

};

}