
#pragma once

#include <vector>

namespace pde::grid::groups {

/**
 * Implementation of a DSU of a set of integers 0..n-1
 * Initially all integers belong to a separate group
 */
class DisjointUnion {

public:

    /**
     * Constructor
     * @param n The number of elements in the DSU
     */
    DisjointUnion(size_t n);

    /**
     * Get a representative of the group that the given integer is in
     * @param x The integer to get the group of
     * @return A representative (will be the same for all elements in the
     * group) of the group that x is in
     */
    size_t find(size_t x);

    /**
     * Join two groups (could already be in the same group) given by two
     * elements
     * @param x The first integer
     * @param y The second integer
     */
    void join(size_t x, size_t y);

    /**
     * Get the size of a group
     * @param x An integer in the group
     * @return The size of the group that the integer is a part of
     */
    size_t groupSize(size_t x);

private:

    // The parent of each integer in the DSU tree, will be the element itself
    // at initialization
    std::vector<size_t> parent;

    // Stores the sizes of all of the groups, note that it only stores the
    // correct size in the index of the group representative
    std::vector<size_t> sizes;

};

}