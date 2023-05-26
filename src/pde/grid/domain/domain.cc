
#include "domain.h"
#include "expr/expr.h"
#include "util.h"
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace pde::grid::domain;

Domain::Domain() { }

Domain::~Domain() { }

void Domain::setDims(const std::vector<std::string> &val) {
    dims = val;
}

void Domain::setPivot(const std::vector<double> &val) {
    pivot = val;
}

void Domain::setScale(double val) {
    scale = val;
}

void Domain::setExpr(const expr::ExprNode &val) {
    expr = val;
    // Replace dimension names with markers
    std::unordered_map<std::string, size_t> dimMap;
    for (size_t i = 0; i < dims.size(); i++)
        dimMap.emplace(dims[i], i);
    expr.replaceSymbols(dimMap);
}

void Domain::setMaxSize(size_t val) {
    maxSize = val;
}

void Domain::setSpread(const std::vector<size_t> &val) {
    spread = val;
}

size_t Domain::size() const {
    return cells.size() + borderCells.size();
}

void Domain::run() {
    determineDomain();
    determineBorder();
    normalize();
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
        pivot[i] += range[i].first * scale;
}

std::vector<std::pair<long, long>> Domain::getRange() const {
    std::vector<std::pair<long, long>> range(dims.size(), {0, 0});
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

std::vector<double> Domain::toPoint(const std::vector<long> &loc) const {
    std::vector<double> out(loc.size());
    for (size_t i = 0; i < loc.size(); i++)
        out[i] = pivot[i] + scale * loc[i];
    return out;
}

bool Domain::inDomain(const std::vector<long> &loc) const {
    return inDomain(toPoint(loc));
}

bool Domain::inDomain(const std::vector<double> &point) const {
    static const double EPS = 1e-10;
    return abs(expr.evalDirect(point)) > EPS;
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
    const std::vector<long> zero(dims.size(), 0);
    if (!inDomain(zero))
        std::runtime_error("Pivot not in domain");
    std::queue<std::vector<long>> locs;
    locs.push(zero);
    while (!locs.empty() && size() <= maxSize) {
        std::vector<long> cur = locs.front();
        locs.pop();
        for (const std::vector<long> &nb : getNeighbours(cur)) {
            if (inDomain(nb)) {
                locs.push(nb);
                cells.insert(nb);
            }
        }
    }
    if (size() > maxSize)
        throw std::runtime_error("Maximum domain size exceeded");
}

void Domain::determineBorder() {
    for (size_t i = 0; i < spread.size(); i++) {
        std::vector<std::vector<long>> cur, next;
        for (const std::vector<long> &cell : cells)
            cur.push_back(cell);
        for (const std::vector<long> &cell : borderCells)
            cur.push_back(cell);
        for (size_t j = 0; j < spread[i] && !cur.empty(); j++) {
            for (const std::vector<long> &c : cur) {
                for (const std::vector<long> &nb : getNeighbours(c)) {
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