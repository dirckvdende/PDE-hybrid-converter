
#include "exprgrid.h"
#include <stdexcept>
#include <vector>

ExprGrid::ExprGrid(std::vector<size_t> dims) : HyperGrid<ExprGridPoint>(dims)
{ }

ExprGrid::~ExprGrid() { }

std::string ExprGrid::str() const {
    // TODO: Implement
    return "";
}

void ExprGrid::generatePointExpr() {
    for (size_t i = 0; i < size(); i++)
        generatePointExpr(i);
}

void ExprGrid::generatePointExpr(size_t index) {
    switch (get(index).type) {
        case GRIDPOINT_ERR:
            throw std::runtime_error("Cannot generate grid point expression for"
            "ERR type");
        case GRIDPOINT_NONE:
            // Do nothing
            return;
        case GRIDPOINT_DOMAIN:
            get(index).pointExpr = ExprNode(NODE_SYMB, {}, "outf_" +
            std::to_string(index));
            return;
        case GRIDPOINT_BORDER:
            generateBorderExpr(index);
            return;
    }
}

void ExprGrid::generateBorderExpr(size_t index) {
    // TODO: implement
}

const ExprNode &ExprGrid::getPointExpr(std::vector<size_t> pos) const {
    return get(pos).pointExpr;
}

ExprNode ExprGrid::getApprox(std::vector<size_t> deriv, std::vector<size_t> pos)
const {
    // TODO: Implement
    return ExprNode();
}