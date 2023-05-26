
#pragma once

#include "cell.h"
#include "hypergrid/hypergrid.h"
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
     * Set the pivot position of the grid
     * @param val The pivot position
     */
    void setPivot(const std::vector<double> &val);

    /**
     * Set the scale of the grid
     * @param val The new grid cell scale
     */
    void setScale(double val);

private:

    // Pivot point of the grid, this indicates the coordinates of the 0 integer
    // point
    std::vector<double> pivot;
    // The scale of the grid
    double scale;

};

}