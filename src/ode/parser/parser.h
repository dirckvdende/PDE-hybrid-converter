/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "treeparser/parser.h"
#include "ode/spec.h"
#include <string>
#include <vector>

namespace ode::parser {

/**
 * Parses an input ODE configuration file
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
     * Set the text to be parsed
     * @param txt The input text to be parsed
     */
    void setText(const std::string &txt);

    /**
     * Run the parser
     */
    void run();

    /**
     * Get the list of ODE specifications
     * @return A constant reference to the produced ODE specifications
     */
    const std::vector<ODESystem> &getSpecs() const;

    /**
     * Clear both input text and stored specifications
     */
    void clear();

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
     * Parse a variable definition in an ODE specification
     * @param txt The input text corresponding with the variable definition
     * @param system The output system to add the variable to
     */
    void parseVar(const std::string &txt, ODESystem &system);

    /**
     * Parse a interval definition in an ODE specification
     * @param txt The input text corresponding with the interval definition
     * @param system The output system to add the interval to
     */
    void parseInterval(const std::string &txt, ODESystem &system);

    /**
     * Parse an emit definition in an ODE specification
     * @param txt The input text corresponding with the emit definition
     * @param system The output system to add the emit to
     */
    void parseEmit(const std::string &txt, ODESystem &system);

    /**
     * Parse a time definition in an ODE specification
     * @param txt The input text corresponding with the time definition
     * @param system The output system to add the time configuration to
     */
    void parseTime(const std::string &txt, ODESystem &system);

    /**
     * Internal type to store ODE configurations where the values have not yet
     * been parsed
     */
    struct ODEPreConfig {
        // Pairs contain name and value of configuration
        std::vector<std::pair<std::string, std::string>> entries;
    };

    // Input text to be parsed
    std::string txt;
    // Tree parser to read configuration entries as strings
    treeparser::Parser treeParser;
    // List of ODE configurations and configuration values
    std::vector<ODEPreConfig> preConfig;
    // List of ODE specifications
    std::vector<ODESystem> specs;

};

}