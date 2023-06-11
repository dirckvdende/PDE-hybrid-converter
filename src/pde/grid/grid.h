/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "cell.h"
#include "hypergrid/hypergrid.h"
#include "pde/spec.h"
#include <vector>

namespace pde::grid {

/**
 * Basic grid that stores information about what the several components in the
 * compiler have generated
 */
class Grid : public hypergrid::HyperGrid<GridCell> {

public:

    /**
     * Constructor
     */
    Grid();

    /**
     * Destructor
     */
    virtual ~Grid();

    /**
     * Get the real coordinates of a grid cell given by a reference/pointer.
     * This uses the pivot and scale stored in this object
     * @param cell A pointer/reference to the grid cell
     * @return A list, the real coordinates of the cell
     */
    std::vector<double> toRealLoc(const GridCell *cell);
    std::vector<double> toRealLoc(const GridCell &cell);

    // Names of the grid dimensions
    std::vector<std::string> dims;
    // Pivot point of the grid, this indicates the coordinates of the 0 integer
    // point
    std::vector<double> pivot;
    // Grid scale
    double scale;
    // Iteration counter
    size_t iteration;
    // Hardware component limit (abstract)
    size_t componentLimit;
    // Maximum (total) grid size
    size_t maxGridSize;
    // Dependency spread
    std::vector<size_t> spread;
    // Domain definition expression
    expr::ExprNode domain;

private:

};

}