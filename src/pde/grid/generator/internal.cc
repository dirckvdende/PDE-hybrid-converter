/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "expr/expr.h"
#include "internal.h"
#include "pde/approx/approx.h"
#include "util.h"
#include <stack>

using namespace pde::grid::generator;

InternalExprGenerator::InternalExprGenerator(Grid &grid, const PDESystem
&system) : ExprGenerator(grid, system), approx(grid) { }

InternalExprGenerator::~InternalExprGenerator() { }

void InternalExprGenerator::generate(GridCell &cell) {
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.vals.clear();
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.vals[i]);
        cell.vals.back().replaceDims(loc);
    }
    replaceAll(cell);
    calcInit(cell);
}

void InternalExprGenerator::replaceAll(GridCell &cell) {
    approx.convert(cell);
    replaceRawVars(cell);
}

void InternalExprGenerator::replaceRawVars(GridCell &cell) {
    for (expr::ExprNode &val : cell.vals) {
        val.walk([&](expr::ExprNode &node) -> void {
            if (node.type == expr::NODE_SYMB && node.content.front() != '_')
                node.content = toGridVar(node.content, grid.toLoc(cell),
                grid.iteration);
        });
    }
}

void InternalExprGenerator::calcInit(GridCell &cell) {
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.init.clear();
    for (size_t i = 0; i < system.init.size(); i++)
        cell.init.push_back(system.init[i].evalDims(loc));
}