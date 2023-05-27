
#include "dsu.h"
#include "groupgrid.h"
#include "pde/grid/hypergrid/hypergrid.h"
#include <ostream>
#include <vector>

using namespace pde::grid::groups;

GroupGrid::GroupGrid(std::vector<size_t> dims, size_t maxSize) :
HyperGrid(dims), dsu(size()), maxSize(maxSize) { }

GroupGrid::~GroupGrid() { }

size_t GroupGrid::group(std::vector<size_t> loc) {
    return dsu.find(toIndex(loc));
}

void GroupGrid::join(std::vector<size_t> locA, std::vector<size_t> locB) {
    if (!canJoin(locA, locB))
        throw std::runtime_error("Attempted to exceed maximum group size");
    dsu.join(toIndex(locA), toIndex(locB));
}

bool GroupGrid::canJoin(std::vector<size_t> locA, std::vector<size_t> locB) {
    size_t a = toIndex(locA), b = toIndex(locB);
    if (dsu.find(a) == dsu.find(b))
        return true;
    return dsu.groupSize(a) + dsu.groupSize(b) <= maxSize;
}

std::ostream &operator<<(std::ostream &os, GroupGrid &grid) {
    if (grid.getShape().size() != 2)
        return os << "[grid]\n";
    for (size_t i = 0; i < grid.getShape()[0]; i++) {
        for (size_t j = 0; j < grid.getShape()[1]; j++)
            os << grid.group({i, j}) << ' ';
        os << '\n';
    }
    return os;
}

size_t GroupGrid::getMaxSize() const {
    return maxSize;
}