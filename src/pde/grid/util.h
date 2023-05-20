
#pragma once

#include <string>
#include <vector>

namespace pde::grid {

/**
 * Convert a grid position to a string
 * @param pos The grid position as a vector
 * @return A string representation of the position
 */
std::string gridToString(const std::vector<size_t> &pos);

/**
 * Convert a function value at a grid position to a string
 * @param func The name of the function
 * @param pos THe grid position as a vector
 * @return A string representation of the function at a grid position
 */
std::string gridFuncToString(std::string func, const std::vector<size_t> &pos);

}