
#include "dbg/dbg.h"
#include "pdegrid.h"
#include "groups/depgrid.h"
#include "groups/alg/alg.h"
#include "groups/alg/optimalrect.h"
#include <string>

using namespace pde::grid;

PDEGrid::PDEGrid() : componentLimit(168) { }

PDEGrid::~PDEGrid() { }

void PDEGrid::generate() {
    setPivot(system.pivot);
    setScale(system.scale);
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
    dbg::log("Generated domain:\n");
    dbg::log(domainStr());
}

void PDEGrid::calcSpread() {
    // TODO: implement
    spread = std::vector<size_t>(system.dims.size(), 1);
}

void PDEGrid::divideGroups() {
    depends.reshape(getShape());
    depends.setMaxSize(componentLimit);
    depends.setSpread(spread);
    groups::alg::OptimalRectAlg alg;
    alg.setGrid(depends);
    alg.run();
    depends.calc();
    for (GridCell &cell : *this) {
        std::vector<size_t> loc = toLoc(cell);
        cell.group = depends.group(loc);
        cell.isStored = depends.hasDepend(loc);
    }
    dbg::log("Number of dependencies: " + std::to_string(
    depends.dependCount()));
    dbg::log("Dependencies:\n");
    dbg::log(depends.str());
    dbg::log("Grid groups:\n");
    dbg::log(depends.GroupGrid::str());
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

std::string PDEGrid::domainStr() const {
    if (getShape().size() != 2)
        return "[domain grid]\n";
    std::string out;
    for (size_t i = 0; i < getShape()[0]; i++) {
        for (size_t j = 0; j < getShape()[1]; j++) {
            switch (operator[]({i, j}).type) {
                case CELL_DOMAIN:
                    out.push_back('.');
                    break;
                case CELL_BORDER:
                    out.push_back('#');
                    break;
                default:
                    out.push_back(' ');
            }
        }
        out.push_back('\n');
    }
    return out;
}
