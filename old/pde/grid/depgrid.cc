
#include "depgrid.h"
#include <iostream>
#include <queue>
#include <sstream>

using namespace pde::grid;

DependGrid::DependGrid(std::vector<size_t> dims, size_t maxSize,
std::vector<size_t> range) : GroupGrid(dims, maxSize), depends(dims), range(range) {
    if (range.empty())
        range = std::vector<size_t>(dims.size(), 0);
}

DependGrid::~DependGrid() { }

void DependGrid::calc() {
    depends.fill(0);
    if (range.size() != getDims().size())
        throw std::runtime_error("Invalid range given");
    for (size_t d = 0; d < range.size(); d++) {
        if (range[d] == 0)
            continue;
        // Go over all cells and mark ones with neighbour different/marked
        std::vector<std::vector<size_t>> cur, next;
        for (size_t i = 0; i < size(); i++) {
            std::vector<size_t> loc = toLoc(i);
            size_t g = group(loc);
            bool mark = false;
            if (loc[d] > 0) {
                loc[d]--;
                if (g != group(loc) || (depends[loc] != 0 && depends[loc] != d + 1))
                    mark = true;
                loc[d]++;
            }
            if (loc[d] + 1 < getDims()[d]) {
                loc[d]++;
                if (g != group(loc) || (depends[loc] != 0 && depends[loc] != d + 1))
                    mark = true;
                loc[d]--;
            }
            if (mark && !depends[loc])
                depends[loc] = d + 1;
            if (depends[loc])
                cur.push_back(loc);
        }
        // Flood fill for extra distance
        for (size_t i = 1; i < range[d]; i++) {
            for (std::vector<size_t> loc : cur) {
                if (loc[d] > 0) {
                    loc[d]--;
                    if (!depends[loc]) {
                        depends[loc] = d + 1;
                        next.push_back(loc);
                    }
                    loc[d]++;
                }
                if (loc[d] + 1 < getDims()[d]) {
                    loc[d]++;
                    if (!depends[loc]) {
                        depends[loc] = d + 1;
                        next.push_back(loc);
                    }
                    loc[d]--;
                }
            }
            cur = next;
            next.clear();
        }
    }
}

bool DependGrid::hasDepend(std::vector<size_t> loc) {
    return depends[loc];
}

size_t DependGrid::dependCount() const {
    size_t c = 0;
    for (size_t i = 0; i < depends.size(); i++)
        if (depends[i])
            c++;
    return c;
}

void DependGrid::printDepend(std::ostream &os) const {
    for (size_t i = 0; i < getDims()[0]; i++) {
        for (size_t j = 0; j < getDims()[1]; j++)
            os << (depends[{i, j}] > 0 ? 1 : 0) << " ";
        os << std::endl;
    }
}

std::vector<size_t> DependGrid::getRange() const {
    return range;
}

void DependGrid::setRange(const std::vector<size_t> &range) {
    this->range = range;
}