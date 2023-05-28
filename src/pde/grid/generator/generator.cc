
#include "generator.h"

using namespace pde::grid::generator;

ExprGenerator::ExprGenerator() : iteration(0) { }

ExprGenerator::~ExprGenerator() { }

void ExprGenerator::setGrid(Grid &gridRef) {
    grid = &gridRef;
}

void ExprGenerator::setSystem(const parser::PDESystem &sys) {
    system = sys;
}

void ExprGenerator::setIteration(size_t val) {
    iteration = val;
}
