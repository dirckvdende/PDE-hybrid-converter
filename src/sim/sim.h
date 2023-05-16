
#pragma once

#include "parser.h"
#include "spec.h"
#include <string>
#include <vector>

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

    /**
     * Run the simulator
     */
    void run();

private:

    // Input text
    std::string txt;
    // Parses the input text
    Parser parser;
    // List of ODE specifications to process
    std::vector<ODESystem> specs;

};

};