/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "presystem.h"
#include <string>
#include <unordered_map>

namespace pde::parser {

/**
 * Transforms the input system to a form that can be processed by the rest of
 * the compiler
 */
class InputTransform {

public:

    /**
     * Constructor
     * @param preSystem A reference to the presystem to transform
     */
    InputTransform(PreSystem &preSystem);

    /**
     * Destructor
     */
    ~InputTransform();

    /**
     * Run the input transformer
     */
    void run();

private:

    /**
     * Collect information about variable names and generate new equations for
     * derivatives
     */
    void genEquations();

    /**
     * Generate new boundary conditions and check if boundary conditions are
     * correct
     */
    void genBoundary();

    /**
     * Check if all initial conditions are present, and transform with new
     * variables
     */
    void genInitial();

    /**
     * Check if all bounds are present, and transform with new variables
     */
    void genBounds();

    /**
     * General function for initial conditions and bounds, to check if all
     * derivatives are present, and to replace derivatives with variable names
     * @param nodes The nodes list that corresponds with either initial
     * conditions or bounds
     */
    void checkAllDerivs(std::vector<expr::ExprNode> &nodes);

    /**
     * Alternative variable name for nth derivative
     * @param var The original variable name
     * @param n The derivative
     * @return The alternative variable name
     */
    static std::string derivName(const std::string &var, size_t n);

    // A reference to the presystem to tranform, note that the system will
    // be modified by this object
    PreSystem &preSystem;
    // List of variables and their derivative counts
    std::unordered_map<std::string, size_t> vars;

};

}