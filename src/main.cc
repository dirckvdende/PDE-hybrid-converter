
// #include "compiler/compiler.h"
#include "dbg/dbg.h"
#include "sim/sim.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * Run the main compiler
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return An integer status code to be returned by main
 */
// int runCompiler(int argc, char *argv[]) {
//     Compiler compiler(argc, argv);
//     compiler.run();
//     return 0;
// }

/**
 * Run the ODE simulator
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return An integer status code to be returned by main
 */
int runSim(int argc, char *argv[]) {
    std::string filename;
    sim::Sim simulator;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.size() >= 1 && arg.front() != '-')
            filename = argv[i];
        if (arg.substr(0, 2) == "-s")
            simulator.setStepSize(std::stod(arg.substr(2)));
    }
    simulator.setFileOutput(true);
    if (filename == "") {
        std::cerr << "Usage: " << argv[0] << " --ode <filename>" << std::endl;
        return 1;
    }
    std::ifstream file(filename);
    std::stringstream stream;
    stream << file.rdbuf();
    std::string txt = stream.str();
    simulator.setText(txt);
    simulator.run();
    return 0;
}

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
    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "--ode")
            return runSim(argc, argv);
    return 1;
    // return runCompiler(argc, argv);
}
