
#include "compiler.h"
#include "dbg/dbg.h"
#include "grid/cell.h"
#include "grid/pdegrid.h"
#include "parser/parser.h"
#include "parser/spec.h"
#include <fstream>
#include <string>

using namespace pde;

Compiler::Compiler() { }

Compiler::~Compiler() { }

void Compiler::setInput(const std::string &val) {
    txt = val;
}

void Compiler::run() {
    parser.setText(txt);
    parser.run();
    system = parser.getSpecs();
    generator.setPDE(system);
    // TODO: implement real compiler settings
    generator.changeSettings(CompilerSettings());
    dbg::log("Generating iteration expressions...");
    generator.run();
    output();
}

void Compiler::output() const {
    // TODO: implement custom output files
    std::ofstream file("tmp/compiler.ode");
    for (const ode::ODESystem &ode : generator.getSystems())
        file << ode.str() << "\n\n";
    file.close();
}