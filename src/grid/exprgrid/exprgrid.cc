
#include "exprgrid.h"
#include <vector>

ExprGrid::ExprGrid(std::vector<size_t> dims) : HyperGrid<ExprGridPoint>(dims)
{ }

ExprGrid::~ExprGrid() { }

std::string ExprGrid::str() const {
    // TODO: Implement
    return "";
}

const ExprNode &ExprGrid::getPointExpr(std::vector<size_t> pos) const {
    return get(pos).pointExpr;
}

ExprNode ExprGrid::getApprox(std::vector<size_t> deriv, std::vector<size_t> pos)
const {
    // TODO: Implement
    return ExprNode();
}