
#include "compiler.h"
#include "dbg/dbg.h"
#include "grid/cell.h"
#include "parser/parser.h"
#include "spec.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace pde;

Compiler::Compiler() : preprocess(system), generator(system) { }

Compiler::~Compiler() { }

void Compiler::run() {
    readInputFile();
    parser.setText(txt);
    parser.run();
    system = parser.getSpecs();
    preprocess.run();
    generator.apply(settings);
    dbg::log("Generating iteration expressions...");
    generator.run();
    if (settings.doSplitter) {
        splitter.setSystems(generator.getSystems());
        splitter.run();
        outputSystems = splitter.getOutputSystems();
    } else {
        outputSystems = generator.getSystems();
    }
    output();
}

void Compiler::readArgs(const std::vector<std::string> &args) {
    for (const std::string &arg : args) {
        if (arg.empty())
            continue;
        if (arg.front() == '-') {
            if (arg.size() <= 1)
                dbg::error("Ill-formed command line argument format");
            switch (arg[1]) {
                case 'd':
                    break;
                case 'o':
                    settings.outputFilename = arg.substr(2);
                    break;
                case 'c':
                    settings.componentLimit = std::stoull(arg.substr(2));
                    break;
                case 'm':
                    settings.maxGridSize = std::stoull(arg.substr(2));
                    break;
                case 'h':
                    showHelp();
                    exit(0);
                    break;
                case 's':
                    settings.doSplitter = false;
                    break;
                default:
                    break;
            }
        } else {
            if (!settings.filename.empty())
                dbg::error("Multiple input files given");
            settings.filename = arg;
        }
    }
}

bool Compiler::settingsValid() const {
    return settings.componentLimit > 0
        && settings.maxGridSize > 0
        && !settings.filename.empty()
        && !settings.outputFilename.empty();
}

void Compiler::output() const {
    std::ofstream file(settings.outputFilename);
    if (!file.is_open())
        dbg::error("Could not create/open file \"" + settings.outputFilename +
        "\"");
    for (const ode::ODESystem &ode : outputSystems)
        file << ode.str() << "\n\n";
    file.close();
}

void Compiler::readInputFile() {
    std::ifstream file(settings.filename);
    if (!file.is_open())
        dbg::error("Could not open file \"" + settings.filename + "\"");
    std::stringstream stream;
    stream << file.rdbuf();
    txt = stream.str();
}

void Compiler::showHelp() {
    std::cerr << "PDE hybrid system converter\n"
        "Created by Dirck van den Ende, Universiteit Leiden\n"
        "GitHub: https://github.com/dirckvdende/PDE-hybrid-converter\n\n"
        "Converts PDE system specifications to discretized ODE system\n"
        "specifications, which can serve as an input for analog computers\n\n"
        "Command line options:\n\n"
        "    -c  Set an abstract component limit, which controls the number\n"
        "        of ODEs that are put in one system\n"
        "    -d  Enable debug mode\n"
        "    -h  Show this help page\n"
        "    -m  Set the maximum number of grid cells used in the\n"
        "        discretization of the PDE system\n"
        "    -o  Set the output filename\n"
        "    -s  Disable the output ODE splitter, which should only be used\n"
        "        when debugging\n\n";
}