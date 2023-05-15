
#include "exprgrid.h"
#include <vector>

ExprGrid::ExprGrid(std::vector<size_t> dims) : HyperGrid<ExprNode *>(dims) {
    // At first the grid should be empty
    fill(nullptr);
}

ExprGrid::~ExprGrid() {
    for (size_t i = 0; i < size(); i++)
        delete get(i);
}

std::string ExprGrid::str() const {
    std::string out;
    for (size_t i = 0; i < size(); i++) {
        if (get(i) != nullptr) {
            out.append(get(i)->str());
            out.push_back('\n');
        }
    }
    return out;
}