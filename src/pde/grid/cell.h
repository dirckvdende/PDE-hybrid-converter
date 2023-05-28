
#pragma once

#include "expr/expr.h"
#include <vector>

namespace pde::grid {

/**
 * Enum to indicate if a grid cell is part of the domain, border, or neither
 */
enum GridCellType {
    CELL_DOMAIN, CELL_BORDER, CELL_OUTSIDE,
};

/**
 * Describes a cell in the generated PDE grid
 */
struct GridCell {
    // Type of the grid cell
    GridCellType type = CELL_OUTSIDE;
    // Group index of the grid cell
    size_t group = 0;
    // Indicates if the value of this grid cell should be stored between
    // iterations
    bool isStored = false;
    // Expressions for the values of the grid cells
    std::vector<expr::ExprNode> vals;
    // Names of the variables in this grid cell
    std::vector<std::string> vars;
};

}