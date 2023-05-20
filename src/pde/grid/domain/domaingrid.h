
#pragma once

#include "pde/grid/hypergrid/hypergrid.h"
#include <vector>

namespace pde::grid::domain {

/**
 * Encodes wether a cell is contained in the grid domain or not
 */
enum DomainInfo {
    DOM_CONTAINED,
    DOM_BOUNDARY,
    DOM_OUTSIDE,
};

/**
 * A grid that contains information about which cells are contained in the given
 * domain and which ones are not
 */
class DomainGrid : public hypergrid::HyperGrid<DomainInfo> {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid (size per dimension)
     */
    DomainGrid(std::vector<size_t> dims);

    /**
     * Destructor
     */
    ~DomainGrid();

    /**
     * Set the pivot of the domain grid (i.e. top left corner coords)
     * @param val The new pivot value
     */
    void setPivot(const std::vector<double> &val);

    /**
     * Set the scale of the grid (i.e. distance between grid cells)
     * @param val The new scale value
     */
    void setScale(double val);

private:

    // Domain pivot (default (0, 0, ...))
    std::vector<double> pivot;
    // Domain grid cell scale (default 1)
    double scale;

};

}