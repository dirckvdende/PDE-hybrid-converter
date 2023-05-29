
#include "generator.h"

using namespace pde::grid;

GridGenerator::GridGenerator() { }

GridGenerator::~GridGenerator() { }

void GridGenerator::setSystem(const PDESystem &sys) {
    system = sys;
}

void GridGenerator::run() {
    // TODO: implement
}