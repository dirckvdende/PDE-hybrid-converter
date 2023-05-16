
#pragma once

#include "parser.h"

namespace sim {

/**
 * Main object used to simulate ODEs
 */
class Sim {

public:

    /**
     * Constructor
     */
    Sim();

    /**
     * Destructor
     */
    ~Sim();

    /**
     * Set the input text
     * @param txt The input text to be processed
     */
    void setText(const std::string &txt);

private:

    // Input text
    std::string txt;
    // Parses the input text
    Parser parser;

};

};