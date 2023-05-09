
#pragma once

#include "token.h"
#include <string>
#include <vector>

/**
 * Parses the input by first parsing the tree, then the individual text
 * components in the tree
 */
class Parser {

public:

    /**
     * Constructor
     * @param txt A reference to the input text, should not change while parser
     * is run
     */
    Parser(const std::string &txt);

    /**
     * Destructor
     */
    ~Parser();

    /**
     * Run the parser
     */
    void run();

private:

    /**
     * Run the tree lexer and parser and store field values in this object
     */
    void runTreeParser();

    /**
     * Convert the equation, boundary, and domain to tokens
     */
    void lexEquations();

    /**
     * Convert equation, boundary, and domain to parse tree vectors
     */
    void parseEquations();

    /**
     * Throw an error for giving an invalid configuration file
     */
    void invalidConfig();

    // Input text reference
    const std::string &txt;
    // The name of the PDE
    std::string systemName;
    // The main differential equation, boundary condition, and domain, as
    // strings
    std::string equation, boundary, domain;
    // Token vectors for equation, boundary, and domain
    std::vector<EquationToken> equationTokens, boundaryTokens, domainTokens;
    // Parsed equations
    std::vector<EquationNode> equationTree, boundaryTree, domainTree;

};