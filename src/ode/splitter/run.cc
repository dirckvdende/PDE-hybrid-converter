
#include "ode/parser/parser.h"
#include "run.h"
#include "splitter.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace ode::splitter;

int ode::splitter::run(int argc, char *argv[]) {
    std::string filename;
    ode::splitter::Splitter splitter;
    ode::parser::Parser parser;
    for (int i = 1; i < argc; i++)
        if (argv[i][0] != '-')
            filename = argv[i];
    if (filename == "") {
        std::cerr << "Usage: " << argv[0] << " --splitter <filename>" <<
        std::endl;
        return 1;
    }
    std::ifstream file(filename);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    std::string txt = stream.str();
    parser.setText(txt);
    parser.run();
    splitter.setSystems(parser.getSpecs());
    splitter.run();
    std::ofstream out("tmp/splitter.ode");
    for (const ODESystem &system : splitter.getOutputSystems())
        out << std::endl << system.str() << std::endl;
    out.close();
    return 0;
}