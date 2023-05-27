
#pragma once

#include "alg.h"
#include "pde/grid/groups/groupgrid.h"
#include <random>
#include <vector>

namespace pde::grid::groups::alg {

class RectSpreadAlg final : public DivisionAlg {

public:

    /**
     * Constructor, inherited
     */
    using DivisionAlg::DivisionAlg;

    /**
     * Destructor
     */
    virtual ~RectSpreadAlg();

    /**
     * Run the "optimal hyperrectangle" division algorithm
     */
    virtual void run();

private:

    /**
     * Get the dimensions of the optimal hyperrectangle
     */
    void calcMaxDims();

    /**
     * Recursive function to join points in all hyperrectangles
     * @param loc The lower corner of the hypercube
     * @param depth The current recusion depth
     */
    void assignAllRects(std::vector<size_t> loc, size_t depth);

    /**
     * Recursive function to move over hyperrectangle and assign every point to
     * the same group
     * @param base The base point with which to join all other points
     * @param loc The current location
     * @param depth The current recursion depth
     */
    void assignRect(std::vector<size_t> base, std::vector<size_t> loc,
    size_t depth);

    /**
     * Perform spread algorithm on remaining cells
     */
    void spread();

    // Dimensions of the hyperrectangle
    std::vector<size_t> rectDims;
    // Keep track of which cells have been added to a group yet
    std::vector<bool> grouped;

};

}