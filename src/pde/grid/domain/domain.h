
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
     * Determine the cells that make up the grid, given the pivot, scale and
     * domain expression
     */
    void run();

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

    // Names of the dimensions of the grid, used to evaluate the domain expression
    std::vector<std::string> dims;
    // Pivot position, which corresponds to point 0 on the grid
    std::vector<double> pivot;
    // The scale of the grid cells
    double scale;
    // The expression that should be evaluated at a grid point to check if it is
    // in the domain
    expr::ExprNode expr;
    // The set of all grid cells belonging to the domain, as currently
    // determined
    std::unordered_set<std::vector<long>, VectorHash> cells;

};

}