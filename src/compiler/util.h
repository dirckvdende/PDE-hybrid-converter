
#pragma once

#include <string>
#include <vector>

/**
 * Strip whitespace at the start and end of a string
 * @param txt The string to strip whitespace of
 * @return The stripped string
 */
std::string stripWhitespace(const std::string &txt);

/**
 * Convert a comma-separated list of strings into a vector
 * @param txt The comma-separated list
 * @return A vector of strings
 * @note Also strips whitespace around entries
 */
std::vector<std::string> splitString(const std::string &txt);