
#pragma once

#include "expr/expr.h"
#include "fields.h"
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
     * Get a reference to the entered input fields, given that run() has been
     * called before
     * @return A reference to an InputFields object containing all input fields
     */
    const InputFields &getInputFields() const;

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
     * Parse a specific expression field
     * @param str A reference to the string version of the expression
     * @param out A reference to the field where the expression should be stored
     */
    void storeExpr(const std::string &str, ExprNode &out);

    /**
     * Parse a specific expression number field
     * @param str A reference to the string version of the expression
     * @param out A reference to the field where the number should be stored
     */
    void storeNumber(const std::string &str, double &out);

    /**
     * Parse a specific comma separated list field
     * @param str A reference to the string version of the expression
     * @param out A reference to the vector where the list should be stored
     */
    void storeCommaList(const std::string &str, std::vector<std::string> &out);

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
    // Input fields as parsed variants
    InputFields inputFields;

};