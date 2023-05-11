
#pragma once

#include "expr/expr.h"
#include "hypergrid.h"
#include <vector>

/**
 * A grid that stores expressions
 */
class ExprGrid : public HyperGrid<ExprNode *> {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     */
    ExprGrid(std::vector<size_t> dims);

    /**
     * Destructor
     */
    ~ExprGrid();

private:

};