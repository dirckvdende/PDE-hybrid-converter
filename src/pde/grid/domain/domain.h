
#pragma once

#include "expr/expr.h"
#include "pde/grid/grid.h"
#include "util.h"
#include <string>
#include <vector>
#include <unordered_set>

namespace pde::grid::domain {

/**
 * Class to determine domain grid cells
 */
class Domain {

public:

    /**
     * Constructor
     * @param grid A reference to the grid to modify
     */
    Domain(Grid &grid);

    /**
     * Destructor
     */
    ~Domain();

    /**
     * Get the number of grid cells currently stored in this object
     * @return The number cells as an integer
     */
    size_t size() const;

    /**
     * Determine the cells that make up the grid, given the pivot, scale and
     * domain expression
     */
    void run();

    /**
     * Normalize the grid to have all stored cells have positive coordinates in
     * each dimension. Note that this function also alters the grid pivot
     */
    void normalize();

    /**
     * Get the minimum and maximum coordinate value of the domain cells in each
     * dimension
     * @return A list of pairs, which contain the minimum and maximum values in
     * each dimension
     */
    std::vector<std::pair<long, long>> getRange() const;

    /**
     * Output the generated domain and border cells to the grid. This function
     * should be called after calculation and normalization of the grid
     */
    void apply() const;

private:

    /**
     * Convert an integer location to a real location in the domain space
     * @param loc The integer location
     * @return The real location
     */
    std::vector<double> toPoint(const std::vector<long> &loc) const;

    /**
     * Check if a point given by an integer location is in the domain (not
     * adjusted for scale and pivot)
     * @param loc The location of the point
     * @return A boolean indicating if the point is in the domain
     */
    bool inDomain(const std::vector<long> &loc) const;

    /**
     * Check if a point is in the domain, given its coordinates in the domain
     * @param point The coordinates of the point
     * @return A boolean indicating if the point is in the domain
     */
    bool inDomain(const std::vector<double> &point) const;

    /**
     * Get all of the neighbours of an integer grid location that have not yet
     * been stored
     * @param loc The location of the grid point to determine the neighbours of
     * @return A list of grid points
     */
    std::vector<std::vector<long>> getNeighbours(const std::vector<long> &loc)
    const;

    /**
     * Check if a grid point has been stored yet
     * @param loc The integer location of the grid point
     * @return A boolean indicating if the grid point is stored
     */
    bool isStored(const std::vector<long> &loc) const;

    /**
     * Determine all of the points inside the domain and store them
     */
    void determineDomain();

    /**
     * Determine the border cells and store them, based on the stored spread
     * range. This function should be run after determineDomain()
     */
    void determineBorder();

    // Grid to modify and use
    Grid &grid;
    // Copy of the domain expression, which is modified with markers
    expr::ExprNode expr;
    // The set of all grid cells belonging to the domain, as currently
    // determined
    std::unordered_set<std::vector<long>, VectorHash> cells;
    // The set of all grid cells on the border of the domain, as currently
    // determined
    std::unordered_set<std::vector<long>, VectorHash> borderCells;

};

}