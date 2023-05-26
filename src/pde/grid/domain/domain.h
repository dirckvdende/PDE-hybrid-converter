
#pragma once

#include "expr/expr.h"
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
     */
    Domain();

    /**
     * Destructor
     */
    ~Domain();

    /**
     * Set the names of the dimensions of the grid
     * @param val The dimension names as a list of strings
     */
    void setDims(const std::vector<std::string> &val);

    /**
     * Set the pivot position of the domain grid, should be done before the
     * calculation is performed
     * @param val The pivot position
     */
    void setPivot(const std::vector<double> &val);

    /**
     * Set the scale of the grid, should be done before the calculation is
     * performed
     * @param val The new grid cell scale
     */
    void setScale(double val);

    /**
     * Set the expression that should be evaluated at each grid point, to check
     * if the point is part of the domain, this should be done before the
     * calculation is performed. Also, this function should be called AFTER
     * setDims()
     * @param val The new expression
     */
    void setExpr(const expr::ExprNode &val);

    /**
     * Change the maximum number of grid cells that should be determined. If
     * this number is exceeded, an error is thrown
     * @param val The new maximum size
     */
    void setMaxSize(size_t val);

    /**
     * Set the spread in each dimension in which the border should spread
     * @param val The new values for the spread in each dimension
     */
    void setSpread(const std::vector<size_t> &val);

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
     * each dimension
     */
    void normalize();

    /**
     * Get the minimum and maximum coordinate value of the domain cells in each
     * dimension
     * @return A list of pairs, which contain the minimum and maximum values in
     * each dimension
     */
    std::vector<std::pair<long, long>> getRange() const;

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

    // Names of the dimensions of the grid, used to evaluate the domain expression
    std::vector<std::string> dims;
    // Pivot position, which corresponds to point 0 on the grid
    std::vector<double> pivot;
    // The scale of the grid cells
    double scale;
    // The expression that should be evaluated at a grid point to check if it is
    // in the domain
    expr::ExprNode expr;
    // The maximum number of grid cells that should be stored
    size_t maxSize;
    // The range in each dimension in which the border should spread
    std::vector<size_t> spread;
    // The set of all grid cells belonging to the domain, as currently
    // determined
    std::unordered_set<std::vector<long>, VectorHash> cells;
    // The set of all grid cells on the border of the domain, as currently
    // determined
    std::unordered_set<std::vector<long>, VectorHash> borderCells;

};

}