
#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace expr {

class ExprNode;

/**
 * Special function definition, which contains information about evaluation and
 * differentiation
 */
struct FuncDef {
    // Evaluation function
    std::function<double(double)> eval;
    // Differentiation function
    std::function<ExprNode *(const ExprNode &)> diff;
};

// Special function list, mapping names to functions to call
extern const std::unordered_map<std::string, FuncDef> funcs;

}