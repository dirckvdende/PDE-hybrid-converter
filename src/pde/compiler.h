
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
     * Run the compiler
     */
    void run();

    /**
     * Process command line arguments to apply compiler settings
     * @param args List of command line arguments
     */
    void readArgs(const std::vector<std::string> &args);

    /**
     * Check if the global compiler settings are valid
     * @return A boolean indicating if the settings are valid
     */
    bool settingsValid() const;

private:

    /**
     * Output the generated ODEs to a file
     */
    void output() const;

    /**
     * Read the input file and set the input text
     */
    void readInputFile();

    /**
     * Show command line help
     */
    static void showHelp();

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
    // Global compiler settings
    CompilerSettings settings;

};

}