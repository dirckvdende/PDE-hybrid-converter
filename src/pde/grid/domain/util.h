/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <vector>

namespace pde::grid::domain {

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

}