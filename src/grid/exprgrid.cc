
#include "exprgrid.h"
#include <vector>

ExprGrid::ExprGrid(std::vector<size_t> dims) : HyperGrid<ExprNode *>(dims) {
    // At first the grid should be empty
    fill(nullptr);
}

ExprGrid::~ExprGrid() { }