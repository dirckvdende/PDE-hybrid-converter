
#include "dbg/dbg.h"
#include "generator.h"
#include "groups/alg/optimalrect.h"
#include "pde/approx/spread.h"

using namespace pde::grid;

GridGenerator::GridGenerator() { }

GridGenerator::~GridGenerator() { }

void GridGenerator::setSystem(const PDESystem &sys) {
    system = sys;
}

void GridGenerator::run() {
    configureGrid();
}

void GridGenerator::configureGrid() {
    grid.system = system;
    grid.pivot = system.pivot;
    grid.iteration = 0;
    // TODO: implement changing component limit
    grid.componentLimit = 168;
}

void GridGenerator::generateDomain() {
    domain.setDims(system.dims);
    domain.setPivot(system.pivot);
    domain.setScale(system.scale);
    domain.setExpr(system.domain);
    domain.setSpread(spread);
    domain.run();
    domain.normalize();
    domain.apply(grid);
    dbg::log("Generated domain:\n");
    dbg::log(domainStr());
}

void GridGenerator::calcSpread() {
    spread = approx::calcSpread(system);
}

void GridGenerator::divideGroups() {
    depends.reshape(grid.getShape());
    depends.setMaxSize(grid.componentLimit);
    depends.setSpread(spread);
    groups::alg::OptimalRectAlg alg;
    alg.setGrid(depends);
    alg.run();
    depends.calc();
    for (GridCell &cell : grid) {
        std::vector<size_t> loc = grid.toLoc(cell);
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

std::string GridGenerator::domainStr() const {
    if (grid.getShape().size() != 2 || grid.size() > 10000)
        return "[domain grid]\n";
    std::string out;
    for (size_t i = 0; i < grid.getShape()[0]; i++) {
        for (size_t j = 0; j < grid.getShape()[1]; j++) {
            switch (grid[{i, j}].type) {
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