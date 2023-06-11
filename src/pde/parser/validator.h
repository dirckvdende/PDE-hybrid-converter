/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

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
     * Determine all variable names and dimension names and check for duplicates
     */
    void findNames();

    /**
     * Check emit validity, uses the variable list
     */
    void checkEmits();

    /**
     * Check if a derivative node only has "t" derivatives
     * @param node The derivative node
     */
    static bool onlyTimeDeriv(const expr::ExprNode &node);

    /**
     * Check if a derivative contains a "t" derivative
     * @param node The derivative node
     */
    static bool hasTimeDeriv(const expr::ExprNode &node);

    /**
     * Check if variable references on the right of the equations are valid,
     * uses the variable list
     */
    void checkVarRefs();

    /**
     * Check if the derivatives on the right of the equations are all valid
     */
    void checkRightDerivs();

    // A constant reference to the presystem to validate
    const PreSystem &preSystem;
    // List of variable and dimension names
    std::unordered_set<std::string> varNames, dimNames;

};

}