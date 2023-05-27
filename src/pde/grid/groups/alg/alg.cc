
#include "alg.h"
#include "pde/grid/groups/groupgrid.h"

using namespace pde::grid::groups::alg;

DivisionAlg::DivisionAlg() : grid(nullptr) { }

DivisionAlg::DivisionAlg(DependGrid &depGrid) : grid(&depGrid) { }

DivisionAlg::~DivisionAlg() { }

void DivisionAlg::setGrid(DependGrid &depGrid) {
    grid = &depGrid;
}