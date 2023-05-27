
#pragma once

#include "alg.h"

namespace pde::grid::groups::alg {

class NoneAlg final : public DivisionAlg {

public:

    /**
     * Constructor, inherited
     */
    using DivisionAlg::DivisionAlg;

    /**
     * Destructor
     */
    virtual ~NoneAlg();

    /**
     * Run the "optimal hyperrectangle" division algorithm
     */
    virtual void run();

private:

};

}