
#include "boundary.h"
#include "expr/expr.h"
#include <unordered_map>

using namespace pde;
using namespace pde::grid;
using namespace pde::grid::generator;

BoundaryExprGenerator::~BoundaryExprGenerator() { }

void BoundaryExprGenerator::generate(GridCell &cell) {
    std::vector<double> loc = grid.toRealLoc(cell);
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.boundary[i]);
        cell.vals.back().replaceDims(loc);
    }
}