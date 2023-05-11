
#pragma once

#include "cmdargs.h"
#include "expr/expr.h"
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
     * Output debug text, if debug mode is enabled
     * @param txt The debug text to display
     */
    void debugLog(std::string txt) const;

    // Command line argument parser
    CmdArgs args;
    // Input file contents
    std::string fileContents;
    // Configuration field values as strings
    std::unordered_map<std::string, std::string> fieldValues;
    // Field values as expressions (if applicable)
    std::unordered_map<std::string, ExprNode *> fieldExpr;

};