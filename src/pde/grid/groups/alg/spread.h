
#pragma once

#include "alg.h"
#include "pde/grid/groups/groupgrid.h"
#include <random>

namespace pde::grid::groups::alg {

class SpreadAlg final : public DivisionAlg {

public:

    /**
     * Constructor, inherited
     */
    using DivisionAlg::DivisionAlg;

    /**
     * Destructor
     */
    virtual ~SpreadAlg();

    /**
     * Run the "spread" division algorithm
     */
    virtual void run();

private:

};

}