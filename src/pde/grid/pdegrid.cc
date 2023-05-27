
#include "dbg/dbg.h"
#include "pdegrid.h"
#include "groups/depgrid.h"
#include "groups/alg/alg.h"
#include "groups/alg/optimalrect.h"

using namespace pde::grid;

PDEGrid::PDEGrid() : componentLimit(168) { }

PDEGrid::~PDEGrid() { }

void PDEGrid::generate() {
    generateDomain();
    calcSpread();
    divideGroups();
    generateBorderExpressions();
    generateDomainExpressions();
    // TODO: implement
}

void PDEGrid::setComponentLimit(size_t val) {
    componentLimit = val;
}

void PDEGrid::setSystem(const parser::PDESystem &val) {
    system = val;
}

void PDEGrid::generateDomain() {
    domain.setDims(system.dims);
    domain.setPivot(system.pivot);
    domain.setScale(system.scale);
    domain.setExpr(system.domain);
    domain.setSpread(spread);
    domain.setMaxSize(1000);
    domain.run();
    domain.normalize();
    domain.apply(*this);
}

void PDEGrid::calcSpread() {
    // TODO: implement
    spread = std::vector<size_t>(system.vars.size(), 1);
}

void PDEGrid::divideGroups() {
    depends.reshape(getShape());
    depends.setMaxSize(componentLimit);
    depends.setSpread(spread);
    groups::alg::OptimalRectAlg alg;
    alg.setGrid(depends);
    alg.run();
    for (GridCell &cell : *this) {
        std::vector<size_t> loc = toLoc(cell);
        cell.group = depends.group(loc);
        cell.isStored = depends.hasDepend(loc);
    }
    dbg::log("\nDependencies:\n");
    dbg::log(depends.str());
    dbg::log("Grid groups:\n");
    dbg::log(((groups::GroupGrid)depends).str());
}

void PDEGrid::generateBorderExpressions() {
    // TODO: implement
    for (GridCell &cell : *this)
        if (cell.type == CELL_BORDER)
            generateBorderExpression(cell);
}

void PDEGrid::generateBorderExpression(GridCell &cell) {
    // TODO: implement
    // TODO: change such that border specification is better
}

void PDEGrid::generateDomainExpressions() {
    // TODO: implement
}
