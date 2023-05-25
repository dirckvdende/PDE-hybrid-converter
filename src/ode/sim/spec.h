
#pragma once

#include "expr/expr.h"

#include <string>
#include <vector>

namespace ode::sim {

/**
 * Specification of a system of ODEs
 */
struct ODESystem {
    // List of ODEs/variables (names of variables)
    std::vector<std::string> vars;
    // Expression for each variable
    std::vector<expr::ExprNode> vals;
    // Export specification
    std::vector<std::pair<std::string, std::string>> emit;
    // Bounds on the variables
    std::vector<std::pair<double, double>> bounds;
    // Time duration
    double time;
};

}