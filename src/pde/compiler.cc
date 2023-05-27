
#include "compiler.h"
#include "grid/pdegrid.h"
#include "parser/parser.h"
#include "parser/spec.h"
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
    grid.setMaxGridSize(100000);
    grid.setSystem(system);
    grid.setComponentLimit(168);
    grid.generate();
    // TODO: implement
}