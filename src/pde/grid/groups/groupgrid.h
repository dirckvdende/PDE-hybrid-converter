
#pragma once

#include "dsu.h"
#include "pde/grid/hypergrid/hypergrid.h"
#include <ostream>
#include <variant>
#include <vector>

namespace pde::grid::groups {

/**
 * A grid that keeps track of the groups of grid cells
 * At first all grid cells are part of a separate group
 */
class GroupGrid : public hypergrid::HyperGrid<std::monostate> {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param maxSize The maximum size of a group
     */
    GroupGrid(std::vector<size_t> dims, size_t maxSize = SIZE_MAX);

    /**
     * Destructor
     */
    ~GroupGrid();

    /**
     * Get the group number of a grid point
     * @param loc The location of the grid point
     * @return The group number of the point
     */
    size_t group(std::vector<size_t> loc);

    /**
     * Join the groups of two grid points
     * @param locA The location of the first grid point
     * @param locB The location of the second grid point
     */
    void join(std::vector<size_t> locA, std::vector<size_t> locB);

    /**
     * Check if two groups can be joined or if their sizes exceed the maximum
     * group size
     * @param locA The location of the first grid point
     * @param locB The location of the second grid point
     * @return A boolean indicating if the groups of the grid points can be
     * joined, returns true if they are already in the same group
     */
    bool canJoin(std::vector<size_t> locA, std::vector<size_t> locB);

    /**
     * Prints out the groups of the grid to the given output stream. Only prints
     * if the grid is 2D
     * @param os The output stream
     * @param grid The grid to output
     * @return The output stream
     */
    friend std::ostream &operator<<(std::ostream &os, GroupGrid &grid);

    /**
     * Get the maximum size of a group
     * @return The maximum number of grid points that can be in a group
     */
    size_t getMaxSize() const;

private:

    // A dsu to keep track of the groups of the grid points
    DisjointUnion dsu;
    // The maximum group size
    const size_t maxSize = SIZE_MAX;

};

}