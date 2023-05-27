
#include "dsu.h"
#include "groupgrid.h"
#include "pde/grid/hypergrid/hypergrid.h"
#include <ostream>
#include <vector>

using namespace pde::grid::groups;

GroupGrid::GroupGrid() : dsu(nullptr), maxSize(SIZE_MAX) { }

GroupGrid::GroupGrid(const std::vector<size_t> &dims, size_t maxSize) :
HyperGrid(dims), dsu(new DisjointUnion(size())), maxSize(maxSize) { }

GroupGrid::~GroupGrid() {
    delete dsu;
}

size_t GroupGrid::group(const std::vector<size_t> &loc) {
    return dsu->find(toIndex(loc));
}

void GroupGrid::join(const std::vector<size_t> &locA, const std::vector<size_t>
&locB) {
    if (!canJoin(locA, locB))
        throw std::runtime_error("Attempted to exceed maximum group size");
    dsu->join(toIndex(locA), toIndex(locB));
}

bool GroupGrid::canJoin(const std::vector<size_t> &locA, const
std::vector<size_t> &locB) {
    size_t a = toIndex(locA), b = toIndex(locB);
    if (dsu->find(a) == dsu->find(b))
        return true;
    return dsu->groupSize(a) + dsu->groupSize(b) <= maxSize;
}

std::string GroupGrid::str() {
    if (getShape().size() != 2 || size() > 1000)
        return "[grid]\n";
    std::string out;
    for (size_t i = 0; i < getShape()[0]; i++) {
        for (size_t j = 0; j < getShape()[1]; j++)
            out.append(std::to_string(group({i, j})) + " ");
        out.push_back('\n');
    }
    return out;
}

void GroupGrid::setMaxSize(size_t val) {
    maxSize = val;
}

size_t GroupGrid::getMaxSize() const {
    return maxSize;
}

void GroupGrid::reshape(const std::vector<size_t> &val) {
    HyperGrid<std::monostate>::reshape(val);
    delete dsu;
    dsu = new DisjointUnion(size());
}

void GroupGrid::clear() {
    HyperGrid<std::monostate>::clear();
    delete dsu;
    dsu = nullptr;
}

void GroupGrid::reset() {
    delete dsu;
    dsu = new DisjointUnion(size());
}