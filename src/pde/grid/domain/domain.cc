
#include "domain.h"
#include "expr/expr.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace pde::grid::domain;

Domain::Domain() { }

Domain::~Domain() { }

void Domain::setDims(const std::vector<std::string> &val) {
    dims = val;
}

void Domain::setPivot(const std::vector<double> &val) {
    pivot = val;
}

void Domain::setScale(double val) {
    scale = val;
}

void Domain::setExpr(const expr::ExprNode &val) {
    expr = val;
    // Replace dimension names with markers
    std::unordered_map<std::string, size_t> dimMap;
    for (size_t i = 0; i < dims.size(); i++)
        dimMap.emplace(dims[i], i);
    expr.replaceSymbols(dimMap);
}

void Domain::run() {
    // TODO: implement
}

bool Domain::inDomain(const std::vector<double> &point) const {
    static const double EPS = 1e-10;
    return abs(expr.evalDirect(point)) > EPS;
}