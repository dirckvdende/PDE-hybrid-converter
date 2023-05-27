
#pragma once

#include "groupgrid.h"
#include "pde/grid/hypergrid/hypergrid.h"
#include <string>

namespace pde::grid::groups {

/**
 * Object that can calculate which grid cells depend on ones from other groups
 * given a certain range per dimension
 */
class DependGrid : public GroupGrid {

public:

    /**
     * Constructor
     */
    DependGrid();

    /**
     * Constructor
     * @param dims The dimensions of the grid
     */
    DependGrid(const std::vector<size_t> &dims);

    /**
     * Destructor
     */
    ~DependGrid();

    /**
     * Set the range per dimension that dependencies span
     * @param val The new range per dimension
     */
    void setSpread(const std::vector<size_t> &val);

    /**
     * Determine all grid cells that depend on cells from other groups given a
     * certain range per dimension
     * @post The result is stored in this object
     */
    void calc();

    /**
     * Check if a certain cell is dependent on cells from other gorups,
     * according to the last calc() call
     * @param loc The location of the cell
     * @return A boolean indicating if the the cell is dependent on other groups
     */
    bool hasDepend(const std::vector<size_t> &loc);

    /**
     * Get the number of cells that depend on cells from other groups, according
     * to the last calc() call
     * @return The amount of cells that depend on a cell from another group
     */
    size_t dependCount() const;

    /**
     * Get a string representation of the dependency grid, with 1 if a cell
     * depends on another, and 0 otherwise. Only works for 2D grids
     * @return The string representation
     */
    std::string str() const;

    /**
     * Get the range per dimension of the dependencies
     * @return The range per dimension as a vector
     */
    std::vector<size_t> getSpread() const;

private:

    // Grid to store result from calc() function, where the entry corresponds to
    // the iteration at which it was marked, or 0 if it hasn't been marked
    hypergrid::HyperGrid<size_t> depends;

    // The range per dimension that dependencies span
    std::vector<size_t> spread;

};

}