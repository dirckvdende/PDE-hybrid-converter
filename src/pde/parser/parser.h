
#pragma once

#include "spec.h"
#include "treeparser/parser.h"
#include <string>
#include <vector>

namespace pde::parser {

/**
 * Parses PDE text input to specification
 */
class Parser {

public:

    /**
     * Constructor
     */
    Parser();

    /**
     * Destructor
     */
    ~Parser();

    /**
     * Set the text to be parser
     * @param txt THe input text to be parsed
     */
    void setText(const std::string &txt);

    /**
     * Run the parser
     */
    void run();

    /**
     * Get the PDE system specification that was parsed
     * @return A constant reference to the system specification
     */
    const PDESystem &getSpecs() const;

private:

    /**
     * Run the tree parser
     */
    void runTreeParser();

    /**
     * Run expression parser to parse fields produced by runTreeParser()
     */
    void runExprParser();

    /**
     * Parse a single number from a string
     * @param txt The text to parse
     * @return The parsed number as a double
     */
    double parseNum(const std::string &txt) const;

    // Input text to be parsed
    std::string txt;
    // Tree parser to convert raw text to text field list
    treeparser::Parser treeParser;
    // PDE specification as a vector of pairs: contain name and value of setting
    std::vector<std::pair<std::string, std::string>> preConfig;
    // Output specification fields
    PDESystem system;

};

}