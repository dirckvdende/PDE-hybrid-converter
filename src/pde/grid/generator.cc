
#include "dbg/dbg.h"
#include "generator.h"
#include "generator/util.h"
#include "groups/alg/optimalrect.h"
#include "pde/approx/spread.h"
#include <unordered_map>

using namespace pde::grid;

GridGenerator::GridGenerator() : domain(grid), boundaryGen(grid, system),
internalGen(grid, system) { }

GridGenerator::~GridGenerator() { }

void GridGenerator::setSystem(const PDESystem &sys) {
    system = sys;
    grid.dims = system.dims;
    grid.pivot = system.pivot;
    grid.scale = system.scale;
    grid.iteration = 0;
    // TODO: implement custom component limits/grid sizes
    grid.componentLimit = 168;
    grid.maxGridSize = 1000000;
    grid.domain = system.domain;
    // Replace dimension names with markers in variable expressions
    std::unordered_map<std::string, size_t> dimMap;
    for (size_t i = 0; i < system.dims.size(); i++)
        dimMap.emplace(system.dims[i], i);
    for (expr::ExprNode &node : system.vals)
        node.replaceSymbols(dimMap);
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
    grid.spread = approx::calcSpread(system);
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
    depends.setSpread(grid.spread);
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