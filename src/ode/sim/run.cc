
#include "run.h"
#include "sim.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace ode::sim;

bool ode::sim::checkHelp(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++)
        if (argv[i] == std::string("-h"))
            return true;
    return false;
}

void ode::sim::showHelp() {
    std::cout <<
        "ODE specification simulator. Takes as an input an ODE specification\n"
        "file, and outputs data from the \"emit\" variables to a CSV file\n"
        "\"tmp/ode.csv\"." << std::endl;
}

int ode::sim::run(int argc, char *argv[]) {
    std::string filename;
    ode::sim::Sim simulator;
    if (checkHelp(argc, argv)) {
        showHelp();
        return 0;
    }
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