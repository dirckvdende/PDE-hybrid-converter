
#pragma once

#include "expr/expr.h"
#include <vector>
#include <string>

/**
 * An object to store all input fields in different formats
 */
struct InputFields {
    std::string str() const;
    size_t iterations;
    double time;
    expr::ExprNode init;
    expr::ExprNode boundary;
    double scale;
    std::vector<double> pivot;
    expr::ExprNode domain;
    expr::ExprNode func;
    expr::ExprNode equation;
    std::vector<std::string> dimensions;
};