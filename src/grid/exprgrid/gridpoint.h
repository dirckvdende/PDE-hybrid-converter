
#pragma once

#include "expr/expr.h"

/**
 * Expression grid point domain inclusion type
 */
enum ExprGridPointType {
    // Not inside the domain
    GRIDPOINT_NONE,
    // Inside the domain
    GRIDPOINT_DOMAIN,
    // On the border of the domain
    GRIDPOINT_BORDER,
};

/**
 * Cell in the expression grid, encoding information about being inside the
 * domain, expression for referencing grid point, etc.
 */
struct ExprGridPoint {
    ExprGridPointType type;
    ExprNode *pointExpr = nullptr;
};