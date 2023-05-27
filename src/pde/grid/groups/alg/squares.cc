
#include "pde/grid/groups/groupgrid.h"
#include "squares.h"
#include <cmath>
#include <random>
#include <vector>
#include <iostream>

using namespace pde::grid::groups::alg;

SquaresAlg::~SquaresAlg() { }

void SquaresAlg::run() {
    calcMaxDim();
    std::vector<size_t> zero(grid->getShape().size(), 0);
    assignAllCubes(zero, 0);
}

void SquaresAlg::calcMaxDim() {
    maxDim = 1;
    while (std::pow(maxDim + 1, grid->getShape().size()) <= grid->getMaxSize())
        maxDim++;
}

void SquaresAlg::assignAllCubes(std::vector<size_t> loc, size_t depth) {
    if (depth == grid->getShape().size()) {
        assignCube(loc, loc, 0);
        return;
    }
    while (loc[depth] < grid->getShape()[depth]) {
        assignAllCubes(loc, depth + 1);
        loc[depth] += maxDim;
    }
}

void SquaresAlg::assignCube(std::vector<size_t> base, std::vector<size_t> loc,
size_t depth) {
    if (depth == grid->getShape().size()) {
        if (grid->canJoin(base, loc))
            grid->join(base, loc);
        return;
    }
    for (size_t i = 0; i < maxDim && loc[depth] < grid->getShape()[depth]; i++) {
        assignCube(base, loc, depth + 1);
        loc[depth]++;
    }
}