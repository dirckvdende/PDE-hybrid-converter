/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "pde/grid/groups/groupgrid.h"
#include "squares.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace pde::grid::groups::alg;

SquaresAlg::~SquaresAlg() { }

void SquaresAlg::run() {
    calcMaxDim();
    std::vector<size_t> zero(grid->getShape().size(), 0);
    assignAllCubes(zero, 0);
}

void SquaresAlg::calcMaxDim() {
    double root = std::pow(grid->getMaxSize(), 1.0 / grid->getShape().size());
    maxDim = std::max(1UL, size_t(root));
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
    for (size_t i = 0; i < maxDim && loc[depth] < grid->getShape()[depth]; i++)
    {
        assignCube(base, loc, depth + 1);
        loc[depth]++;
    }
}