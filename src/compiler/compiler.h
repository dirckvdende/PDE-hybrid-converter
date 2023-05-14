
#pragma once

#include "cmdargs.h"
#include "expr/expr.h"
#include "grid/domain.h"
#include "parser/fields.h"
#include <string>
#include <unordered_map>

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

    /**
     * Run the input text parser
     */
    void runParser();

    /**
     * Determine the domain given in the input file
     */
    void findDomain();

    /**
     * Output debug text, if debug mode is enabled
     * @param txt The debug text to display
     */
    void debugLog(std::string txt) const;

    // Command line argument parser
    CmdArgs args;
    // Input file contents
    std::string fileContents;
    // Domain object, which is null at initialization
    GridDomain *domain;
    // Input fields as determined by the parser
    InputFields inputFields;

};