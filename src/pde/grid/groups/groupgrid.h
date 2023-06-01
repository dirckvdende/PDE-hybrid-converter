
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
     */
    GroupGrid();

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param maxSize The maximum size of a group
     */
    GroupGrid(const std::vector<size_t> &dims, size_t maxSize = SIZE_MAX);

    /**
     * Destructor
     */
    virtual ~GroupGrid();

    /**
     * Get the group number of a grid point
     * @param loc The location of the grid point
     * @return The group number of the point
     */
    size_t group(const std::vector<size_t> &loc);

    /**
     * get the group number of an index
     * @param index The index that corresponds with a grid point
     * @return The group number of the index point
     */
    size_t group(size_t index);

    /**
     * Join the groups of two grid points
     * @param locA The location of the first grid point
     * @param locB The location of the second grid point
     */
    void join(const std::vector<size_t> &locA, const std::vector<size_t> &locB);

    /**
     * Check if two groups can be joined or if their sizes exceed the maximum
     * group size
     * @param locA The location of the first grid point
     * @param locB The location of the second grid point
     * @return A boolean indicating if the groups of the grid points can be
     * joined, returns true if they are already in the same group
     */
    bool canJoin(const std::vector<size_t> &locA, const std::vector<size_t>
    &locB);

    /**
     * Get a string representation of the grouped grid, if it is 2D
     * @return The string representation
     */
    std::string str();

    /**
     * Set the maximum group size
     * @param val The new maximum group size
     */
    void setMaxSize(size_t val);

    /**
     * Get the maximum size of a group
     * @return The maximum number of grid points that can be in a group
     */
    size_t getMaxSize() const;

    /**
     * Alternative reshape function for derived class, to also update DSU
     * @param val The new shape of the grid
     * @note When the grid is reshaped, all cells are placed in separate groups
     * again!
     */
    virtual void reshape(const std::vector<size_t> &val);

    /**
     * Alternative clear function to also update DSU
     */
    virtual void clear();

    /**
     * Reset the groups the grid cells are a part of
     */
    virtual void reset();

private:

    // A dsu to keep track of the groups of the grid points
    DisjointUnion *dsu;
    // The maximum group size
    size_t maxSize;

};

}