
#pragma once

#include "cmdargs.h"

/**
 * Main compiler component
 */
class Compiler {

public:

    /**
     * Constructor
     * @param argc Number of command line argument
     * @param argv Command line arguments
     * @note Command line arguments are parsed upon creation of Compiler object
     */
    Compiler(int argc, char *argv[]);

    /**
     * Destructor
     */
    ~Compiler();

    /**
     * Run the compiler
     */
    void run();

private:

    /**
     * Read input file into fileContents variable
     */
    void readInputFile();

    // Command line argument parser
    CmdArgs args;
    // Input file contents
    std::string fileContents;

};