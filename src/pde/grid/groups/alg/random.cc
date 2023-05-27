
#include "pde/grid/groups/groupgrid.h"
#include "random.h"
#include <random>

using namespace pde::grid::groups::alg;

RandomAlg::~RandomAlg() { }

void RandomAlg::run() {
    size_t fails = 0;
    while (fails < failLimit) {
        auto locs = randomNeighbours();
        std::vector<size_t> locA = locs.first, locB = locs.second;
        // Choose random neighbour
        if (grid->group(locA) == grid->group(locB))
            ;
        else if (!grid->canJoin(locA, locB))
            fails++;
        else
            grid->join(locA, locB);
    }
}

std::pair<std::vector<size_t>, std::vector<size_t>>
RandomAlg::randomNeighbours() const {
    std::random_device device;
    std::mt19937 rng = std::mt19937(device());
    std::vector<size_t> out(grid->getShape().size());
    // Choose random starting location
    for (size_t i = 0; i < grid->getShape().size(); i++) {
        std::uniform_int_distribution<std::mt19937::result_type> distr(0,
        grid->getShape()[i] - 1);
        out[i] = distr(rng);
    }
    // Choose a random direction until a valid one is reached
    std::vector<size_t> nb = out;
    bool isValid = false;
    while (!isValid) {
        std::uniform_int_distribution<std::mt19937::result_type> distr(0,
        grid->getShape().size() - 1);
        size_t d = distr(rng);
        bool m = rand() % 2;
        if ((m && out[d] == 0) || (!m && out[d] + 1 >= grid->getShape()[d]))
            continue;
        nb[d] = m ? nb[d] - 1 : nb[d] + 1;
        isValid = true;
    }
    return {out, nb};
}