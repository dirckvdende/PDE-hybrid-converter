
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
    GridCellType type;
    std::vector<expr::ExprNode> vals;
};

}