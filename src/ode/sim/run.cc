
#include "run.h"
#include "sim.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace ode::sim;

int ode::sim::run(int argc, char *argv[]) {
    std::string filename;
    ode::sim::Sim simulator;
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