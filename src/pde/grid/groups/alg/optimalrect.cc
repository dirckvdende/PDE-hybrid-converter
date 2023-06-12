/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "pde/grid/groups/groupgrid.h"
#include "optimalrect.h"
#include <cmath>
#include <random>
#include <vector>
#include <iostream>

using namespace pde::grid::groups::alg;

OptimalRectAlg::~OptimalRectAlg() { }

void OptimalRectAlg::run() {
    calcMaxDims();
    std::vector<size_t> zero(grid->getShape().size(), 0);
    assignAllRects(zero, 0);
}

void OptimalRectAlg::calcMaxDims() {
    size_t N = grid->getMaxSize();
    std::vector<size_t> reduce = grid->getSpread();
    for (size_t &r : reduce)
        r *= 2;
    std::vector<std::pair<std::vector<size_t>, double>> M(N + 1, {{}, 1.0});
    for (size_t d = 0; d < grid->getShape().size(); d++) {
        auto Mp = M;
        for (size_t n = 0; n <= N; n++) {
            Mp[n].second = 0.0;
            Mp[n].first.push_back(1);
            if (n > reduce[d] && n > 1) {
                for (size_t x = std::max(reduce[d], 1UL); x <= n; x++) {
                    double val = M[n / x].second * double(x - reduce[d]) /
                    double(x);
                    if (val > Mp[n].second) {
                        Mp[n].second = val;
                        Mp[n].first = M[n / x].first;
                        Mp[n].first.push_back(x);
                    }
                }
            }
        }
        M = Mp;
    }
    rectDims = M[N].first;
    dbg::log("Optimal rectangle ratio: " + std::to_string(M[N].second * 100.0) +
    "%");
    std::string dimString;
    bool first = true;
    for (size_t d : rectDims) {
        if (!first)
            dimString.append(", ");
        dimString.append(std::to_string(d));
        first = false;
    }
    dbg::log("Optimal rectangle dimensions: (" + dimString + ")");
}

void OptimalRectAlg::assignAllRects(std::vector<size_t> loc, size_t depth) {
    if (depth == grid->getShape().size()) {
        assignRect(loc, loc, 0);
        return;
    }
    while (loc[depth] < grid->getShape()[depth]) {
        assignAllRects(loc, depth + 1);
        loc[depth] += rectDims[depth];
    }
}

void OptimalRectAlg::assignRect(std::vector<size_t> base, std::vector<size_t>
loc, size_t depth) {
    if (depth == grid->getShape().size()) {
        if (grid->canJoin(base, loc))
            grid->join(base, loc);
        return;
    }
    for (size_t i = 0; i < rectDims[depth] && loc[depth] <
    grid->getShape()[depth]; i++) {
        assignRect(base, loc, depth + 1);
        loc[depth]++;
    }
}