
#pragma once

#include "spec.h"
#include "treeparser/parser.h"
#include <string>
#include <unordered_map>
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
     * Check if all required fields are given in the preConfig. Throws an error
     * if this is not the case. Also checks if configurations have the correct
     * lengths
     */
    void checkRequiredFields() const;

    /**
     * Parse a single number from a string
     * @param txt The text to parse
     * @return The parsed number as a double
     */
    double parseNum(const std::string &txt) const;

    /**
     * Parse an equation, the result will be output to the PDE system
     * @param txt The text to parse
     */
    void parseEquation(const std::string &txt);

    // Input text to be parsed
    std::string txt;
    // Tree parser to convert raw text to text field list
    treeparser::Parser treeParser;
    // PDE specification as a map: maps a name to a vector of values
    std::unordered_map<std::string, std::vector<std::string>> preConfig;
    // Output specification fields
    PDESystem system;

};

}