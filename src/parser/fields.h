
#pragma once

#include "expr/expr.h"
#include <vector>
#include <string>

/**
 * An object to store all input fields in different formats
 */
struct InputFields {
    size_t iterations;
    double time;
    ExprNode init;
    ExprNode boundary;
    double scale;
    std::vector<double> pivot;
    ExprNode domain;
    ExprNode func;
    ExprNode equation;
    std::vector<std::string> dimensions;
};