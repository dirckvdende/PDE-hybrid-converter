
#pragma once

#include "grid/pdegrid.h"
#include "ode.h"
#include "ode/spec.h"
#include "parser/parser.h"
#include "parser/spec.h"
#include <string>

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
    // The input PDE system
    parser::PDESystem system;
    // PDE grid to generate expressions on a grid
    grid::PDEGrid grid;
    // ODE system generator
    ODEGenerator generator;

};

}