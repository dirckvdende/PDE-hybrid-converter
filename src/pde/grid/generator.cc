
#include "dbg/dbg.h"
#include "generator.h"
#include "generator/util.h"
#include "groups/alg/optimalrect.h"
#include "pde/approx/spread.h"

using namespace pde::grid;

GridGenerator::GridGenerator() : domain(grid), boundaryGen(grid, system),
internalGen(grid, system) { }

GridGenerator::~GridGenerator() { }

void GridGenerator::setSystem(const PDESystem &sys) {
    system = sys;
}

void GridGenerator::prepare() {
    calcSpread();
    generateDomain();
    divideGroups();
}

void GridGenerator::run(size_t iteration) {
    dbg::log("Generating expressions for iteration " + std::to_string(
    iteration));
    grid.iteration = iteration;
    generateNames();
    generateExpr();
}

void GridGenerator::calcSpread() {
    spread = approx::calcSpread(system);
}

void GridGenerator::generateDomain() {
    domain.run();
    domain.normalize();
    domain.apply();
    dbg::log("Generated domain:\n");
    dbg::log(domainStr());
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

void GridGenerator::generateNames() {
    for (GridCell &cell : grid) {
        cell.vars.clear();
        for (size_t i = 0; i < system.vars.size(); i++)
            cell.vars.push_back(generator::toGridVar(system.vars[i],
            grid.toLoc(cell), grid.iteration));
    }
}

void GridGenerator::generateExpr() {
    for (GridCell &cell : grid) {
        if (cell.type == CELL_BORDER)
            boundaryGen.generate(cell);
        else if (cell.type == CELL_DOMAIN)
            internalGen.generate(cell);
    }
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