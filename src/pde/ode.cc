
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "grid/cell.h"
#include "grid/generator.h"
#include "grid/grid.h"
#include "ode.h"
#include "spec.h"
#include <unordered_set>

using namespace pde;

ODEGenerator::ODEGenerator(const PDESystem &system) : pde(system) { }

ODEGenerator::~ODEGenerator() { }

void ODEGenerator::changeSettings(const CompilerSettings &val) {
    settings = val;
}

void ODEGenerator::run() {
    grid::GridGenerator gen;
    gen.setSystem(pde);
    gen.grid.maxGridSize = settings.maxGridSize;
    gen.grid.componentLimit = settings.componentLimit;
    gen.prepare();
    for (size_t iteration = 0; iteration < pde.iterations; iteration++) {
        gen.run(iteration);
        if (iteration == 0 && hasTimeReference(gen.grid))
            addTimeSystem();
        systems.emplace_back();
        ode::ODESystem &ode = systems.back();
        for (grid::GridCell &cell : gen.grid) {
            if (cell.type == grid::CELL_BORDER || cell.type == grid::CELL_DOMAIN) {
                ode.vars.insert(ode.vars.end(), cell.vars.begin(),
                cell.vars.end());
                ode.vals.insert(ode.vals.end(), cell.vals.begin(), cell.vals.end());
                if (cell.isStored)
                    for (const std::string &var : cell.vars)
                        ode.emit.push_back({var, var});
                ode.bounds.insert(ode.bounds.begin(), pde.bounds.begin(),
                pde.bounds.end());
            }
        }
        ode.time = pde.time;
    }
}

const std::vector<ode::ODESystem> &ODEGenerator::getSystems() const {
    return systems;
}

bool ODEGenerator::hasTimeReference(const grid::Grid &grid) const {
    std::unordered_set<std::string> symbs;
    for (const grid::GridCell &cell : grid)
        for (const expr::ExprNode &node : cell.vals)
            node.findAllSymbols(symbs);
    return symbs.find("t") != symbs.end();
}

void ODEGenerator::addTimeSystem() {
    ode::ODESystem system;
    system.vars = {"t"};
    system.vals = {expr::ExprNode(expr::NODE_INTEG, {
        new expr::ExprNode(expr::NODE_NUM, {}, 1.0),
        new expr::ExprNode(expr::NODE_NUM, {}, 0.0),
    })};
    system.emit = {{"t", "t"}};
    system.bounds = {{0.0, pde.time}};
    system.time = pde.time;
    systems.push_back(system);
}