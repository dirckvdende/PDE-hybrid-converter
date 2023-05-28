
#include "generator.h"

using namespace pde::grid::generator;

ExprGenerator::ExprGenerator() { }

ExprGenerator::~ExprGenerator() { }

void ExprGenerator::setGrid(Grid &gridRef) {
    grid = &gridRef;
}

void ExprGenerator::setSystem(const parser::PDESystem &sys) {
    system = sys;
}
