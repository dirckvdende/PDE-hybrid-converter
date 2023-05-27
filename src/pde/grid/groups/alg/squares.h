
#pragma once

#include "alg.h"
#include "pde/grid/groups/groupgrid.h"
#include <random>
#include <vector>

namespace pde::grid::groups::alg {

class SquaresAlg final : public DivisionAlg {

public:

    /**
     * Constructor, inherited
     */
    using DivisionAlg::DivisionAlg;

    /**
     * Destructor
     */
    virtual ~SquaresAlg();

    /**
     * Run the "largest hypercube" division algorithm
     */
    virtual void run();

private:

    /**
     * Get the dimensions of the largest hypercube that has at most the maximum
     * volume
     */
    void calcMaxDim();

    /**
     * Recursive function to join points in all hypercubes
     * @param loc The lower corner of the hypercube
     * @param depth The current recusion depth
     */
    void assignAllCubes(std::vector<size_t> loc, size_t depth);

    /**
     * Recursive function to move over hypercube and assign every point to the
     * same group
     * @param base The base point with which to join all other points
     * @param loc The current location
     * @param depth The current recursion depth
     */
    void assignCube(std::vector<size_t> base, std::vector<size_t> loc,
    size_t depth);

    // Dimension of the hypercube
    size_t maxDim;

};

}