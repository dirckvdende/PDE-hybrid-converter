
#include "pde/approx/approx.h"
#include "dbg/dbg.h"
#include "internal.h"
#include "util.h"
#include <stack>

using namespace pde::grid::generator;

InternalExprGenerator::InternalExprGenerator(Grid &grid, const PDESystem
&system) : ExprGenerator(grid, system), approx(grid) { }

InternalExprGenerator::~InternalExprGenerator() { }

void InternalExprGenerator::generate(GridCell &cell) {
    // TODO: implement approximation replacement
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.vals.clear();
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.vals[i]);
        cell.vals.back().replaceDims(loc);
    }
    replaceAll(cell);
    calcInit(cell);
}

void InternalExprGenerator::replaceAll(GridCell &cell) {
    approx.convert(cell);
}

void InternalExprGenerator::calcInit(GridCell &cell) {
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.init.clear();
    for (size_t i = 0; i < system.init.size(); i++)
        cell.init.push_back(system.init[i].evalDims(loc));
}