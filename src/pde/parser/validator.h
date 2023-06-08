
#pragma once

#include "presystem.h"

namespace pde::parser {

/**
 * Object used to validate input format. This is done after the tree parser is
 * run
 */
class InputValidator {

public:

    /**
     * Constructor
     * @param preSystem A constant reference to the presystem to validate
     */
    InputValidator(const PreSystem &preSystem);

    /**
     * Destructor
     */
    ~InputValidator();

    /**
     * Run the input validator. The program will halt if the system is invalid
     */
    void run();

private:

    // A constant reference to the presystem to validate
    const PreSystem &preSystem;

};

}