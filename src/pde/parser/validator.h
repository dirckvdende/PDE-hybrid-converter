
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

    /**
     * Check if all of the expression entries are in the right format
     */
    void checkExpressionFormats();

    /**
     * Check emit validity
     */
    void checkEmits();

    /**
     * Check if a derivative node only has "t" derivatives
     * @param node The derivative node
     */
    static bool onlyTimeDeriv(const expr::ExprNode &node);

    // A constant reference to the presystem to validate
    const PreSystem &preSystem;

};

}