
#ifndef DEPGRID_H
#define DEPGRID_H

#include "groupgrid.h"
#include "hypergrid.h"
#include <string>

/**
 * Object that can calculate which grid cells depend on ones from other groups
 * given a certain range per dimension
 */
class DependGrid : public GroupGrid {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param maxSize The maximum group size
     * @param range The range per dimension that dependencies span, default 0 in
     * all dimensions
     */
    DependGrid(std::vector<size_t> dims, size_t maxSize = SIZE_MAX,
    std::vector<size_t> range = {});

    /**
     * Destructor
     */
    ~DependGrid();

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
    bool hasDepend(std::vector<size_t> loc);

    /**
     * Get the number of cells that depend on cells from other groups, according
     * to the last calc() call
     * @return The amount of cells that depend on a cell from another group
     */
    size_t dependCount() const;

    /**
     * Print the depends property, used for debugging
     * @param os The output stream to print to
     * @warning ONLY WORKS FOR 2D GRIDS
     */
    void printDepend(std::ostream &os) const;

    /**
     * Get the range per dimension of the dependencies
     * @return The range per dimension as a vector
     */
    std::vector<size_t> getRange() const;

    /**
     * Set the range per dimension of the dependencies
     * @param range The new range per dimension
     */
    void setRange(const std::vector<size_t> &range);

private:

    // Grid to store result from calc() function, where the entry corresponds to
    // the iteration at which it was marked, or 0 if it hasn't been marked
    HyperGrid<size_t> depends;

    // The range per dimension that dependencies span
    std::vector<size_t> range;

};

#endif