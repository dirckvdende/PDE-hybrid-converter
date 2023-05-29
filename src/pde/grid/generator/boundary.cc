
#include "boundary.h"
#include "expr/expr.h"
#include <unordered_map>

using namespace pde;
using namespace pde::grid;
using namespace pde::grid::generator;

BoundaryExprGenerator::BoundaryExprGenerator() { }

BoundaryExprGenerator::~BoundaryExprGenerator() { }

void BoundaryExprGenerator::setSystem(const PDESystem &sys) {
    ExprGenerator::setSystem(sys);
    // Replace dimension names with index markers
    std::unordered_map<std::string, size_t> indexMap;
    for (size_t i = 0; i < system.dims.size(); i++)
        indexMap.emplace(system.dims[i], i);
    for (expr::ExprNode &node : system.boundary) {
        node.replaceSymbols(indexMap);
        if (node.containsNonTimeVars())
            throw std::runtime_error("Boundary expression contains invalid "
            "names");
    }
}

void BoundaryExprGenerator::generate(GridCell &cell) {
    std::vector<double> loc = grid->toRealLoc(cell);
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.boundary[i]);
        cell.vals.back().replaceDirect(loc);
    }
}