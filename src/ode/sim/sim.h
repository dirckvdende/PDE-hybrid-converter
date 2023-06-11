/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "data.h"
#include "ode/parser/parser.h"
#include "ode/spec.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ode::sim {

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
     * Set the step size of the simulation
     * @param val The new value for the step size
     */
    void setStepSize(double val);

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
    void runSystem(ODESystem system);

    /**
     * Replace the variables in all expressions in a system with indices from
     * the ODE dataframe
     * @param system The system in which to replace the variable references
     */
    void replaceVars(ODESystem &system);

    /**
     * Generate initial condition values for the given system. The data in the
     * dataframe will be updated
     * @param system The system being simulated
     * @param dataIndex Can retrieve dataframe indices of the variables in the
     * current system
     */
    void generateInitCond(const ODESystem &system, const std::vector<size_t>
    &dataIndex);

    /**
     * Execute a simulation step for the given ODE system. The data in the
     * dataframe will be updated
     * @param system The system being simulated
     * @param step The step index
     * @param dataIndex Can retrieve dataframe indices of the variables in the
     * current system
     */
    void runStep(const ODESystem &system, size_t step, const std::vector<size_t>
    &dataIndex);

    /**
     * Output the emit values to a file
     * @param filename The name of the file to output to
     * @param resolution The (approximate) number of data points to output per
     * emit variable
     */
    void outputEmit(std::string filename, size_t resolution);

    /**
     * Reset simulation statistics
     */
    void resetStats();

    /**
     * Ouput statistics to debug
     */
    void logStats() const;

    // Input text
    std::string txt;
    // Parses the input text
    parser::Parser parser;
    // List of ODE specifications to process
    const std::vector<ODESystem> &specs;
    // Indicates if the results should be output to a text file
    bool fileOutput;
    // Simulation step size
    double stepSize;
    // Simulation statistics
    struct {
        size_t iterations;
        size_t systemCount;
        size_t emitCount;
        double parseTime;
        double iterationTime;
        double systemTime;
        double fileTime;
    } stats;
    // Object used to keep track of emit and variable data
    SimData dataframe;

};

}