/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "domain.h"
#include "expr/expr.h"
#include "pde/grid/cell.h"
#include "pde/grid/grid.h"
#include "util.h"
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <iostream>

using namespace pde::grid::domain;

Domain::Domain(Grid &grid) : grid(grid) { }

Domain::~Domain() { }

size_t Domain::size() const {
    return cells.size() + borderCells.size();
}

void Domain::run() {
    expr = grid.domain;
    determineDomain();
    determineBorder();
}

void Domain::normalize() {
    std::vector<std::pair<long, long>> range = getRange();
    std::unordered_set<std::vector<long>, VectorHash> cellsCopy = cells,
    borderCellsCopy = borderCells;
    cells.clear();
    borderCells.clear();
    for (std::vector<long> cell : cellsCopy) {
        for (size_t i = 0; i < cell.size(); i++)
            cell[i] -= range[i].first;
        cells.insert(cell);
    }
    for (std::vector<long> cell : borderCellsCopy) {
        for (size_t i = 0; i < cell.size(); i++)
            cell[i] -= range[i].first;
        borderCells.insert(cell);
    }
    for (size_t i = 0; i < range.size(); i++)
        grid.pivot[i] += range[i].first * grid.scale;
}

std::vector<std::pair<long, long>> Domain::getRange() const {
    std::vector<std::pair<long, long>> range(grid.dims.size(), {0, 0});
    for (const std::vector<long> &cell : cells) {
        for (size_t i = 0; i < cell.size(); i++) {
            range[i].first = std::min(range[i].first, cell[i]);
            range[i].second = std::max(range[i].second, cell[i]);
        }
    }
    for (const std::vector<long> &cell : borderCells) {
        for (size_t i = 0; i < cell.size(); i++) {
            range[i].first = std::min(range[i].first, cell[i]);
            range[i].second = std::max(range[i].second, cell[i]);
        }
    }
    return range;
}

void Domain::apply() const {
    std::vector<std::pair<long, long>> range = getRange();
    std::vector<size_t> shape;
    for (const std::pair<long, long> &r : range) {
        if (r.first < 0)
            dbg::error("Domain not normalized before apply()");
        shape.push_back(size_t(r.second) + 1);
    }
    // Check if the size of the shape will be too large
    size_t shapeSize = 1;
    for (const size_t &s : shape)
        shapeSize *= s;
    if (shapeSize > grid.maxGridSize)
        dbg::error("Domain too large to process");
    grid.reshape(shape);
    for (size_t index = 0; index < grid.size(); index++) {
        std::vector<long> lpt;
        for (const size_t &p : grid.toLoc(index))
            lpt.push_back(long(p));
        if (cells.find(lpt) != cells.end())
            grid[index].type = CELL_DOMAIN;
        else if (borderCells.find(lpt) != borderCells.end())
            grid[index].type = CELL_BORDER;
        else
            grid[index].type = CELL_OUTSIDE;
    }
}

std::vector<double> Domain::toPoint(const std::vector<long> &loc) const {
    std::vector<double> out(loc.size());
    for (size_t i = 0; i < loc.size(); i++)
        out[i] = grid.pivot[i] + grid.scale * loc[i];
    return out;
}

bool Domain::inDomain(const std::vector<long> &loc) const {
    return inDomain(toPoint(loc));
}

bool Domain::inDomain(const std::vector<double> &point) const {
    static const double EPS = 1e-10;
    return abs(expr.evalDims(point)) > EPS;
}

std::vector<std::vector<long>> Domain::getNeighbours(const std::vector<long>
&loc) const {
    std::vector<std::vector<long>> out;
    for (size_t i = 0; i < loc.size(); i++) {
        static const std::vector<long> diffs = {-1, 1};
        for (long diff : diffs) {
            std::vector<long> nb = loc;
            nb[i] += diff;
            if (!isStored(nb))
                out.push_back(nb);
        }
    }
    return out;
}

bool Domain::isStored(const std::vector<long> &loc) const {
    return cells.find(loc) != cells.end() || borderCells.find(loc) !=
    borderCells.end();
}

void Domain::determineDomain() {
    const std::vector<long> zero(grid.dims.size(), 0);
    if (!inDomain(zero))
        dbg::error("Pivot not in domain");
    std::queue<std::vector<long>> locs;
    locs.push(zero);
    while (!locs.empty() && size() <= grid.maxGridSize) {
        std::vector<long> cur = locs.front();
        locs.pop();
        for (const std::vector<long> &nb : getNeighbours(cur)) {
            if (inDomain(nb)) {
                locs.push(nb);
                cells.insert(nb);
            }
        }
    }
    if (size() > grid.maxGridSize)
        dbg::error("Maximum domain size exceeded");
}

void Domain::determineBorder() {
    for (size_t i = 0; i < grid.spread.size(); i++) {
        std::vector<std::vector<long>> cur, next;
        for (const std::vector<long> &cell : cells)
            cur.push_back(cell);
        for (const std::vector<long> &cell : borderCells)
            cur.push_back(cell);
        for (size_t j = 0; j < grid.spread[i] && !cur.empty(); j++) {
            for (const std::vector<long> &c : cur) {
                std::vector<long> pos = c, neg = c;
                pos[i]++, neg[i]--;
                for (const std::vector<long> &nb :
                std::vector<std::vector<long>>{pos, neg}) {
                    if (!isStored(nb)) {
                        next.push_back(nb);
                        borderCells.insert(nb);
                    }
                }
            }
            cur = next;
            next.clear();
        }
    }
}