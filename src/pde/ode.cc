
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "grid/cell.h"
#include "grid/grid.h"
#include "ode.h"
#include "spec.h"
#include <unordered_set>

using namespace pde;

ODEGenerator::ODEGenerator() { }

ODEGenerator::~ODEGenerator() { }

void ODEGenerator::changeSettings(const CompilerSettings &val) {
    settings = val;
}

void ODEGenerator::setPDE(const PDESystem &sys) {
    pde = sys;
}

void ODEGenerator::run() {
    grid::PDEGrid grid;
    grid.setSystem(pde);
    grid.setMaxGridSize(settings.maxGridSize);
    grid.setComponentLimit(settings.componentLimit);
    grid.generate();
    for (size_t iteration = 0; iteration < pde.iterations; iteration++) {
        dbg::log("Processing iteration #" + std::to_string(iteration) + " out "
        "of " + std::to_string(pde.iterations));
        grid.setIteration(iteration);
        grid.generateExpressions();
        if (iteration == 0 && hasTimeReference(grid))
            addTimeSystem();
        systems.emplace_back();
        ode::ODESystem &ode = systems.back();
        for (grid::GridCell &cell : grid) {
            if (cell.type == grid::CELL_BORDER || cell.type == grid::CELL_DOMAIN) {
                ode.vars.insert(ode.vars.begin(), cell.vars.begin(),
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

bool ODEGenerator::hasTimeReference(const grid::PDEGrid &grid) const {
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