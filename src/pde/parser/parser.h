
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

    /**
     * Parse an assignment expression
     * @param txt The text to parse
     * @param entry The entry in the PDE system to add the expression to
     */
    void parseAssignmentExpr(const std::string &txt, std::vector<expr::ExprNode>
    &entry);

    /**
     * Parse an interval configuration setting. The result is output to the PDE
     * system
     * @param txt The text to parse
     */
    void parseInterval(const std::string &txt);

    /**
     * Parse dimensions configuration settings. The result is output to the PDE
     * system
     * @param txt The text to parse
     */
    void parseDims(const std::string &txt);

    /**
     * Parse a single expression
     * @param txt The text to parse
     * @return The parsed expression node
     */
    expr::ExprNode parseExpr(const std::string &txt) const;

    /**
     * Parse a number list
     * @param txt The text to parse
     * @return A list of doubles, which is the parsed version of the text
     */
    std::vector<double> parseNumList(const std::string &txt) const;

    /**
     * Locate the index of a variable with the given name
     * @param var The name of the variable
     * @return The index of the variable, or SIZE_MAX if not found
     */
    size_t varIndex(const std::string &var) const;

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