
#include "compiler.h"
#include "dbg/dbg.h"
#include "grid/cell.h"
#include "parser/parser.h"
#include "spec.h"
#include <fstream>
#include <string>

using namespace pde;

Compiler::Compiler() : generator(system) { }

Compiler::~Compiler() { }

void Compiler::setInput(const std::string &val) {
    txt = val;
}

void Compiler::run() {
    parser.setText(txt);
    parser.run();
    system = parser.getSpecs();
    // TODO: implement real compiler settings
    generator.changeSettings(CompilerSettings());
    dbg::log("Generating iteration expressions...");
    generator.run();
    splitter.setSystems(generator.getSystems());
    splitter.run();
    outputSystems = splitter.getOutputSystems();
    output();
}

void Compiler::output() const {
    // TODO: implement custom output files
    std::ofstream file("tmp/compiler.ode");
    for (const ode::ODESystem &ode : outputSystems)
        file << ode.str() << "\n\n";
    file.close();
}