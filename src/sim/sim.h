
#pragma once

#include "parser.h"
#include "spec.h"
#include <string>
#include <unordered_map>
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

    /**
     * Set the file output switch
     * @param val The new value
     */
    void setFileOutput(bool val);

    /**
     * Set the simulation step size
     * @param val The new value
     */
    void setStepSize(bool val);

private:

    /**
     * Run parser
     */
    void runParser();

    /**
     * Simulate a specific ODE system
     * @param system The system specification
     */
    void runSystem(const ODESystem &system);

    /**
     * Output the emit values to a file
     * @param filename The name of the file to output to
     * @param resolution The (approximate) number of data points to output per
     * emit variable
     */
    void outputEmit(std::string filename, size_t resolution) const;

    // Input text
    std::string txt;
    // Parses the input text
    Parser parser;
    // List of ODE specifications to process
    const std::vector<ODESystem> &specs;
    // Indicates if the results should be output to a text file
    bool fileOutput;
    // Simulation step size
    double stepSize;
    // "Emit" values stored after simulating the ODEs
    std::unordered_map<std::string, std::vector<double>> emitVals;

};

}