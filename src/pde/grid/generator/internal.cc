
#include "internal.h"

using namespace pde::grid::generator;

InternalExprGenerator::InternalExprGenerator() { }

InternalExprGenerator::~InternalExprGenerator() { }

void InternalExprGenerator::setSystem(const parser::PDESystem &sys) {
    ExprGenerator::setSystem(sys);
    // Replace dimension names with index markers
    std::unordered_map<std::string, size_t> indexMap;
    for (size_t i = 0; i < system.dims.size(); i++)
        indexMap.emplace(system.dims[i], i);
    for (expr::ExprNode &node : system.vals) {
        node.replaceSymbols(indexMap);
        if (node.containsNonTimeVars())
            throw std::runtime_error("Boundary expression contains invalid "
            "names");
    }
}

void InternalExprGenerator::generate(GridCell &cell) {
    // TODO: implement approximation replacement
    std::vector<double> loc = grid->toRealLoc(cell);
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.vals[i]);
        cell.vals.back().replaceDirect(loc);
    }
}