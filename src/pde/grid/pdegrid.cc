
#include "pdegrid.h"

using namespace pde::grid;

PDEGrid::PDEGrid() { }

PDEGrid::~PDEGrid() { }

void PDEGrid::generate() {
    generateDomain();
    generateBorderExpressions();
    generateDomainExpressions();
    // TODO: implement
}

void PDEGrid::generateDomain() {
    domain.setDims(system.dims);
    domain.setPivot(system.pivot);
    domain.setScale(system.scale);
    domain.setExpr(system.domain);
    // TODO: implement border spread
    domain.setSpread(std::vector<size_t>(system.vars.size(), 1));
    domain.run();
    domain.normalize();
    domain.apply(*this);
}

void PDEGrid::generateBorderExpressions() {
    // TODO: implement
    for (GridCell &cell : *this)
        if (cell.type == CELL_BORDER)
            generateBorderExpression(cell);
}

void PDEGrid::generateBorderExpression(GridCell &cell) {
    // TODO: change such that border specification is better

}

void PDEGrid::generateDomainExpressions() {
    // TODO: implement
}
