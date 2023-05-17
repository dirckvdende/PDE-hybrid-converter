
#pragma once

#include "expr/expr.h"

/**
 * Expression grid point domain inclusion type
 */
enum ExprGridPointType {
    // Error type, before types are assigned
    GRIDPOINT_ERR,
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
    ExprGridPointType type = GRIDPOINT_ERR;
    expr::ExprNode pointExpr;
};