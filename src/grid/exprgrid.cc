
#include "exprgrid.h"
#include <vector>

ExprGrid::ExprGrid(std::vector<size_t> dims) : HyperGrid<ExprNode>(dims) {
    // At first the grid should be empty
    fill(ExprNode(NODE_ERR));
}

ExprGrid::~ExprGrid() { }

std::string ExprGrid::str() const {
    std::string out;
    for (size_t i = 0; i < size(); i++) {
        if (operator[](i).type != NODE_ERR) {
            out.append(operator[](i).str());
            out.push_back('\n');
        }
    }
    return out;
}