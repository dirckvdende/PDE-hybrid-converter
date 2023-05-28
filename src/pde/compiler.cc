
#include "compiler.h"
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
    grid.setMaxGridSize(100000);
    grid.setSystem(system);
    grid.setComponentLimit(168);
    grid.generate();
    generateODE();
    // TODO: remove this when iterations are implemented properly
    std::ofstream file("tmp/compiler.ode");
    file << ode.str();
    file.close();
    // TODO: implement iterations
}

void Compiler::generateODE() {
    // TODO: implement iterations
    for (grid::GridCell &cell : grid) {
        if (cell.type == grid::CELL_BORDER || cell.type == grid::CELL_DOMAIN) {
            ode.vars.insert(ode.vars.begin(), cell.vars.begin(),
            cell.vars.end());
            ode.vals.insert(ode.vals.end(), cell.vals.begin(), cell.vals.end());
            if (cell.isStored)
                for (const std::string &var : cell.vars)
                    ode.emit.push_back({var, var});
            ode.bounds.insert(ode.bounds.begin(), system.bounds.begin(),
            system.bounds.end());
        }
    }
    ode.time = system.time;
}