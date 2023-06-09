/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "expr/expr.h"
#include <string>
#include <vector>

namespace ode {

/**
 * Specification of a system of ODEs
 */
struct ODESystem {
    std::string str() const;
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