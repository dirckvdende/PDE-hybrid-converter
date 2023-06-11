/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "expr/expr.h"
#include "pde/spec.h"
#include <vector>

namespace pde::approx {

/**
 * Calculate the maximum spread in each dimension of the given PDE system
 * @param system The PDE system
 * @return A vector of integers, the spread in each dimension
 */
std::vector<size_t> calcSpread(const PDESystem &system);

/**
 * Calculate the maximum spread in each dimension given a variable expression
 * and a map from dimension names to indices
 * @param node A pointer to the expression root node
 * @param nameMap A map that sends dimension names to indices
 * @param out The vector to output the spread to, should be all 0 at the initial
 * call
 */
void calcSpread(const expr::ExprNode *node, const
std::unordered_map<std::string, size_t> &nameMap, std::vector<size_t> &out);

}