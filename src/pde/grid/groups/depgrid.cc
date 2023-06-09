/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "depgrid.h"
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>

using namespace pde;
using namespace pde::grid;
using namespace pde::grid::groups;

DependGrid::DependGrid() { }

DependGrid::DependGrid(const std::vector<size_t> &dims) {
    reshape(dims);
}

DependGrid::DependGrid(const std::vector<size_t> &dims, const
std::vector<size_t> &spread, size_t maxSize) : GroupGrid(dims, maxSize),
spread(spread) {
    reshape(dims);
}

DependGrid::~DependGrid() { }

void DependGrid::setSpread(const std::vector<size_t> &val) {
    spread = val;
}

void DependGrid::calc() {
    if (spread.size() != getShape().size())
        dbg::error("Invalid range given");
    for (size_t d = 0; d < spread.size(); d++) {
        if (spread[d] == 0)
            continue;
        // Go over all cells and mark ones with neighbour different/marked
        std::vector<std::vector<size_t>> cur, next;
        for (size_t i = 0; i < size(); i++) {
            std::vector<size_t> loc = toLoc(i);
            size_t g = group(loc);
            bool mark = false;
            if (loc[d] > 0) {
                loc[d]--;
                if (g != group(loc) || (depends[loc] != 0 && depends[loc] != d +
                1))
                    mark = true;
                loc[d]++;
            }
            if (loc[d] + 1 < getShape()[d]) {
                loc[d]++;
                if (g != group(loc) || (depends[loc] != 0 && depends[loc] != d +
                1))
                    mark = true;
                loc[d]--;
            }
            if (mark && !depends[loc])
                depends[loc] = d + 1;
            if (depends[loc])
                cur.push_back(loc);
        }
        // Flood fill for extra distance
        for (size_t i = 1; i < spread[d]; i++) {
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

bool DependGrid::hasDepend(const std::vector<size_t> &loc) {
    return depends[loc];
}

size_t DependGrid::dependCount() const {
    size_t c = 0;
    for (size_t i = 0; i < depends.size(); i++)
        if (depends[i])
            c++;
    return c;
}

std::string DependGrid::str() const {
    if (getShape().size() > 2 || size() > 10000)
        return "[dependency grid]\n";
    std::string out;
    for (size_t i = 0; i < size(); i++) {
        if (i > 0 && i % getShape()[0] == 0)
            out.push_back('\n');
        out.push_back(depends[i] > 0 ? '1' : '0');
        out.push_back(' ');
    }
    return out;
}

std::vector<size_t> DependGrid::getSpread() const {
    return spread;
}

void DependGrid::reshape(const std::vector<size_t> &val) {
    GroupGrid::reshape(val);
    depends.reshape(val);
    depends.fill(0);
}

void DependGrid::clear() {
    GroupGrid::clear();
    depends.clear();
}