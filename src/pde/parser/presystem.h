/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "expr/expr.h"
#include <string>
#include <vector>

namespace pde::parser {

/**
 * Contains parsed entries from a PDE configuration file
 */
struct PreSystem {
    std::string str() const;
    std::vector<std::string> dims;
    expr::ExprNode domain;
    std::vector<double> pivot;
    double scale;
    std::vector<expr::ExprNode> equations;
    std::vector<expr::ExprNode> inits;
    std::vector<expr::ExprNode> boundaries;
    std::vector<expr::ExprNode> intervals;
    double time;
    size_t iterations;
    std::vector<std::pair<std::string, std::string>> emits;
};

}