
#pragma once

#include <string>

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

    // Input text to be parsed
    std::string txt;

};

}