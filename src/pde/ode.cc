
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "grid/cell.h"
#include "grid/grid.h"
#include "ode.h"
#include <unordered_set>

using namespace pde;

ODEGenerator::ODEGenerator() : grid(nullptr) { }

ODEGenerator::~ODEGenerator() { }

void ODEGenerator::setGrid(grid::PDEGrid &gridRef) {
    grid = &gridRef;
}

void ODEGenerator::setPDE(const parser::PDESystem &sys) {
    pde = sys;
}

void ODEGenerator::run() {
    if (hasTimeReference())
        addTimeSystem();
    for (size_t iteration = 0; iteration < pde.iterations; iteration++) {
        dbg::log("Processing iteration #" + std::to_string(iteration) + " out "
        "of " + std::to_string(pde.iterations));
        grid->setIteration(iteration);
        grid->generateExpressions();
        systems.emplace_back();
        ode::ODESystem &ode = systems.back();
        for (grid::GridCell &cell : *grid) {
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

bool ODEGenerator::hasTimeReference() const {
    std::unordered_set<std::string> symbs;
    for (const grid::GridCell &cell : *grid)
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