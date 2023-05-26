
#pragma once

#include "grid.h"

namespace pde::grid {

/**
 * A derived class of the basic grid that only carries basic information and
 * functions, this one is used by the compiler and has more functionality and
 * interacts with other components
 */
class PDEGrid : public Grid {

public:

    /**
     * Constructor
     */
    PDEGrid();

    /**
     * Destructor
     */
    virtual ~PDEGrid();

private:

};

}