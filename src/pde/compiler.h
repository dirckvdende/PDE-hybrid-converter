
#pragma once

#include "ode.h"
#include "ode/spec.h"
#include "parser/parser.h"
#include "parser/preprocess.h"
#include "ode/splitter/splitter.h"
#include "spec.h"
#include <string>
#include <vector>

namespace pde {

/**
 * Main PDE compiler component
 */
class Compiler {

public:

    /**
     * Constructor
     */
    Compiler();

    /**
     * Destructor
     */
    ~Compiler();

    /**
     * Set the input text
     * @param val The input text
     */
    void setInput(const std::string &val);

    /**
     * Run the compiler
     */
    void run();

private:

    /**
     * Output the generated ODEs to a file
     */
    void output() const;

    // Input text
    std::string txt;
    // The parser used to convert text input into a PDE system
    parser::Parser parser;
    // System preprocessor
    parser::Preprocessor preprocess;
    // The input PDE system
    PDESystem system;
    // ODE system generator
    ODEGenerator generator;
    // ODE splitter, to take apart groups
    ode::splitter::Splitter splitter;
    // Output ODE systems
    std::vector<ode::ODESystem> outputSystems;

};

}