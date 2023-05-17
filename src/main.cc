
#include "compiler/compiler.h"
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
int runCompiler(int argc, char *argv[]) {
    Compiler compiler(argc, argv);
    compiler.run();
    return 0;
}

/**
 * Run the ODE simulator
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return An integer status code to be returned by main
 */
int runSim(int argc, char *argv[]) {
    std::string filename;
    for (int i = 1; i < argc; i++) {
        if (argv[i] != "--ode")
            filename = argv[i];
    }
    sim::Sim simulator;
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

int main(int argc, char *argv[]) {
    // Detect running argument
    for (int i = 1; i < argc; i++)
        if (argv[i] == "--ode")
            return runSim(argc, argv);
    return runCompiler(argc, argv);
}
