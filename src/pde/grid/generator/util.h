/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <string>
#include <vector>

namespace pde::grid::generator {

/**
 * Convert grid coordinates to a variable name
 * @param name The base variable name
 * @param loc The location on the grid
 * @param iteration The iteration counter
 * @return A string representation of the variable at the grid coordinates
 */
std::string toGridVar(const std::string &name, const std::vector<size_t> &loc,
size_t iteration);

}