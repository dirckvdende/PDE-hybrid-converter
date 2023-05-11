
#pragma once

#include "expr/expr.h"
#include <string>
#include <unordered_map>
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

    /**
     * Get the field values read from the configuration file as strings
     * @return A constant reference to a map from field name to field value
     */
    const std::unordered_map<std::string, std::string> &getFieldValues() const;

    /**
     * Get the field expressions read from the configuration file
     * @return A constant reference to a map from field name to field expression
     */
    const std::unordered_map<std::string, ExprNode *> &getFieldExpressions()
    const;

private:

    /**
     * Run the tree lexer and parser and store field values in this object
     */
    void runTreeParser();

    /**
     * Parse expression to ExprNode objects
     */
    void parseExpressions();

    /**
     * Check if all required configuration options are present
     * @return A boolean indicating if all configuration options are present
     */
    bool hasAllFields() const;

    /**
     * Throw an error for giving an invalid configuration file
     */
    void invalidConfig();

    // Input text reference
    const std::string &txt;
    // The name of the PDE
    std::string systemName;
    // Configuration field values as strings
    std::unordered_map<std::string, std::string> fieldValues;
    // Field values as expressions (if applicable)
    std::unordered_map<std::string, ExprNode *> fieldExpr;

};