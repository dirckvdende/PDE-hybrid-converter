/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "ode/sim/run.h"
#include "ode/splitter/run.h"
#include "pde/run.h"
#include <iostream>
#include <string>

/**
 * Check if debug mode argument is given and configure accordingly
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 */
void detectDebugMode(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "-d" || std::string(argv[i]) == "--debug")
            dbg::DEBUG_ENABLED = true;
}

int main(int argc, char *argv[]) {
    detectDebugMode(argc, argv);
    // Detect running argument
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--ode")
            return ode::sim::run(argc, argv);
        if (std::string(argv[i]) == "--splitter")
            return ode::splitter::run(argc, argv);
    }
    return pde::run(argc, argv);
}
