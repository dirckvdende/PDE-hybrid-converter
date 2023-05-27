
#include "depgrid.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>

using namespace pde::grid::groups;

DependGrid::DependGrid(std::vector<size_t> dims, std::vector<size_t> range,
size_t maxSize) : GroupGrid(dims, maxSize), depends(dims), range(range) { }

DependGrid::~DependGrid() { }

void DependGrid::calc() {
    depends.fill(0);
    if (range.size() != getShape().size())
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
            if (loc[d] + 1 < getShape()[d]) {
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
                if (loc[d] + 1 < getShape()[d]) {
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
    for (size_t i = 0; i < getShape()[0]; i++) {
        for (size_t j = 0; j < getShape()[1]; j++)
            os << (depends[{i, j}] > 0 ? 1 : 0) << " ";
        os << std::endl;
    }
}

std::vector<size_t> DependGrid::getRange() const {
    return range;
}