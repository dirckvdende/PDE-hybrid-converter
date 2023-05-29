
#include "internal.h"

using namespace pde::grid::generator;

InternalExprGenerator::~InternalExprGenerator() { }

void InternalExprGenerator::generate(GridCell &cell) {
    // TODO: implement approximation replacement
    std::vector<double> loc = grid.toRealLoc(cell);
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.vals[i]);
        cell.vals.back().replaceDirect(loc);
    }
}

void InternalExprGenerator::generateApprox(GridCell &cell, const
std::vector<size_t> &derivs) {
    // TODO: implement
    (void)cell; (void)derivs;
}