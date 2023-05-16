
#pragma once

#include "treeparser/parser.h"
#include "spec.h"
#include <string>
#include <vector>

namespace sim {

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

private:

    /**
     * Run the tree parser
     */
    void runTreeParser();

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

};

}