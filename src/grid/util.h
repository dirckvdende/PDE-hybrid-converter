
#pragma once

#include <string>
#include <vector>

/**
 * Convert a grid position to a string
 * @param pos The grid position as a vector
 * @return A string representation of the position
 */
std::string gridToString(const std::vector<size_t> &pos);

/**
 * Hasher for vectors
 */
struct VectorHash {
    template<class T>
    size_t operator()(const std::vector<T> &vec) const {
        size_t cur = 0;
        for (const T &i : vec) {
            cur <<= 2;
            cur += std::hash<T>()(i);
        }
        return cur;
    }
};