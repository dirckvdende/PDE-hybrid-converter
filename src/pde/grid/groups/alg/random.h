
#pragma once

#include "alg.h"
#include "pde/grid/groups/groupgrid.h"
#include <random>

namespace pde::grid::groups::alg {

class RandomAlg final : public DivisionAlg {

public:

    /**
     * Constructor, inherited
     */
    using DivisionAlg::DivisionAlg;

    /**
     * Destructor
     */
    virtual ~RandomAlg();

    /**
     * Run the "random pairing" division algorithm
     */
    virtual void run();

private:

    /**
     * Get two random neighbours in the grid
     * @return A pair of location in the gird
     */
    std::pair<std::vector<size_t>, std::vector<size_t>> randomNeighbours()
    const;

    // Limit of the number of failed pairings before the algorithm will halt
    static const size_t failLimit = 2000;

};

}