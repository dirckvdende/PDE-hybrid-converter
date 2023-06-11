/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

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