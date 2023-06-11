/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "generator.h"
#include "generator/util.h"
#include "groups/alg/optimalrect.h"
#include "groups/alg/squares.h"
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
    grid.domain = system.domain;
}

void GridGenerator::prepare() {
    calcSpread();
    generateDomain();
    divideGroups();
}

void GridGenerator::apply(const CompilerSettings &settings) {
    grid.componentLimit = settings.componentLimit;
    grid.maxGridSize = settings.maxGridSize;
}

void GridGenerator::run(size_t iteration) {
    dbg::log("Generating expressions for iteration " + std::to_string(
    iteration));
    grid.iteration = iteration;
    generateNames();
    generateExpr();
    generateEmits();
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
    groups::alg::DivisionAlg *alg;
    bool isZero = true;
    for (size_t i = 0; i < grid.dims.size(); i++)
        if (grid.spread[i] != 0)
            isZero = false;
    if (isZero || grid.componentLimit > 3000)
        alg = new groups::alg::SquaresAlg;
    else
        alg = new groups::alg::OptimalRectAlg;
    alg->setGrid(depends);
    alg->run();
    delete alg;
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
        cell.vals.clear();
        if (cell.type == CELL_BORDER)
            boundaryGen.generate(cell);
        else if (cell.type == CELL_DOMAIN)
            internalGen.generate(cell);
    }
}

void GridGenerator::generateEmits() {
    // Map from (base) variable name to index
    std::unordered_map<std::string, size_t> varIndex;
    for (size_t i = 0; i < system.vars.size(); i++)
        varIndex.emplace(system.vars[i], i);
    for (GridCell &cell : grid) {
        cell.emits.clear();
        if ((cell.type == CELL_DOMAIN || cell.type == CELL_BORDER) &&
        cell.isStored)
            for (const std::string &var : cell.vars)
                cell.emits.push_back({var, var});
        if ((cell.type == CELL_DOMAIN || cell.type == CELL_BORDER) &&
        grid.iteration == system.iterations)
            for (const std::pair<std::string, std::string> &emit : system.emits)
                cell.emits.push_back({cell.vars[varIndex[emit.first]],
                toPosVar(emit.second, cell)});
    }
}

std::string GridGenerator::toPosVar(const std::string &var, const GridCell
&cell) const {
    std::vector<size_t> pos = grid.toLoc(cell);
    std::string out = var;
    for (const size_t &p : pos) {
        std::string num = std::to_string(p);
        out.push_back('_');
        for (size_t i = num.size(); i < 6; i++)
            out.push_back('0');
        out.append(num);
    }
    return out;
}

std::string GridGenerator::domainStr() const {
    if (grid.getShape().size() > 2 || grid.size() > 10000)
        return "[domain grid]\n";
    std::string out;
    for (size_t i = 0; i < grid.size(); i++) {
        if (i > 0 && i % grid.getShape()[0] == 0)
            out.push_back('\n');
        switch (grid[i].type) {
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
    return out;
}