
#include "compiler/compiler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "sim/sim.h"

int main(int argc, char *argv[]) {
    // Compiler compiler(argc, argv);
    // compiler.run();
    // return 0;
    sim::Sim simulator;
    simulator.setFileOutput(true);
    std::ifstream file("examples/ode/single.ode");
    std::stringstream stream;
    stream << file.rdbuf();
    std::string txt = stream.str();
    simulator.setText(txt);
    simulator.run();
    return 0;
}
