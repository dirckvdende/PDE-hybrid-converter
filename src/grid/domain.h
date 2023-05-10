
#pragma once

#include "expr/expr.h"
#include "util.h"
#include <string>
#include <vector>
#include <unordered_set>

/**
 * Object used to determine the cells of a grid that make up a given domain, and
 * auxilliary cells that need to be considered
 * 
 * The domain is assumed to be connected and sufficiently thick, and not too
 * large to exceed the maximum cell count setting
 */
class GridDomain {

public:

    /**
     * Constructor
     * @param scale Grid cell size
     * @param expr An expression that evaluates to true on the domain
     * @param dims The names of the dimensions, in order
     */
    GridDomain(double scale, const ExprNode &expr, const
    std::vector<std::string> &dims);

    /**
     * Determine the grid domain, given some initial point that is contained in
     * the domain
     * @param init The initial point
     */
    void findDomain(const std::vector<double> &init);

    /**
     * Get a string representation of the grid, which is only possible for 2D
     * grids (for debugging). Has a 1 if a point is in the domain, and 0
     * otherwise
     * @return The string representation
     */
    std::string str() const;

private:

    /**
     * Check if a given grid location is inside the domain
     * @param pos The position on the grid
     * @return A boolean indicating if the location is in the domain
     */
    bool inDomain(const std::vector<int> &pos) const;

    /**
     * Convert a grid location (integers) to a real location
     * @param pos The position on the grid
     * @param out Output for the real position
     */
    void convertPos(const std::vector<int> &pos, std::vector<double> &out)
    const;

    // Maximum cells in the domain
    static const size_t maxSize = 5000;
    // Grid cell size
    const double scale;
    // Expression that evaluates to true on the domain
    const ExprNode &expr;
    // The names of the dimensions, in order
    const std::vector<std::string> dims;
    // Initial point inside the domain (used as pivot)
    std::vector<double> init;
    // List of grid cells contained in the domain, not normalized for scale and
    // initial point
    std::unordered_set<std::vector<int>, VectorHash> cells;

};