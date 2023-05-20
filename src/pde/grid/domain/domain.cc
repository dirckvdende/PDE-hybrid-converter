
#include "domain.h"
#include "expr/expr.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace pde::grid::domain;

GridDomain::GridDomain(double scale, const expr::ExprNode &expr, const
std::vector<std::string> &dims) : scale(scale), expr(expr), dims(dims) { }

void GridDomain::findDomain(const std::vector<double> &init) {
    this->init = init;
    cells.clear();
    // Flood-fill to find domain
    std::queue<std::vector<int>> q;
    q.push(std::vector<int>(init.size(), 0));
    cells.insert(q.front());
    while (!q.empty()) {
        std::vector<int> cur = q.front();
        q.pop();
        for (size_t i = 0; i < init.size(); i++) {
            std::vector<int> neg = cur, pos = cur;
            neg[i]--, pos[i]++;
            for (const std::vector<int> &e : {pos, neg}) {
                if (cells.find(e) == cells.end() && inDomain(e)) {
                    if (cells.size() >= maxSize)
                        throw std::runtime_error("Exceeded maximum grid size");
                    q.push(e);
                    cells.insert(e);
                }
            }
        }
    }
}

void GridDomain::findBorder(const std::vector<size_t> &range) {
    borderCells.clear();
    for (size_t i = 0; i < range.size(); i++) {
        std::vector<std::vector<int>> cur, next;
        for (const std::vector<int> &cell : cells)
            cur.push_back(cell);
        for (const std::vector<int> &cell : borderCells)
            cur.push_back(cell);
        for (size_t j = 0; j < range[i] && !cur.empty(); j++) {
            for (const std::vector<int> &c : cur) {
                for (size_t k = 0; k < range.size(); k++) {
                    std::vector<int> neg = c, pos = c;
                    neg[i]--, pos[i]++;
                    for (const std::vector<int> &e : {pos, neg}) {
                        if (cells.find(e) == cells.end() && borderCells.find(e)
                        == borderCells.end()) {
                            next.push_back(e);
                            borderCells.insert(e);
                        }
                    }
                }
            }
            cur = next;
        }
    }
}

std::string GridDomain::str() const {
    if (dims.size() != 2)
        return "";
    std::vector<std::pair<int, int>> ranges = getRanges();
    std::string out;
    for (int x = ranges[0].first; x <= ranges[0].second; x++) {
        for (int y = ranges[1].first; y <= ranges[1].second; y++) {
            if (cells.find({x, y}) != cells.end())
                out.push_back('.');
            else if (borderCells.find({x, y}) != cells.end())
                out.push_back('+');
            else
                out.push_back(' ');
        }
        out.push_back('\n');
    }
    return out;
}

std::vector<std::pair<int, int>> GridDomain::getRanges() const {
    std::vector<std::pair<int, int>> ranges(dims.size(), {0, 0});
    auto update = [&](const std::unordered_set<std::vector<int>, VectorHash>
    &points) -> void {
        for (const std::vector<int> &cell : points) {
            for (size_t i = 0; i < cell.size(); i++) {
                if (cell[i] < ranges[i].first)
                    ranges[i].first = cell[i];
                if (cell[i] > ranges[i].second)
                    ranges[i].second = cell[i];
            }
        }
    };
    update(cells);
    update(borderCells);
    return ranges;
}

void GridDomain::normalize() {
    std::vector<std::pair<int, int>> ranges = getRanges();
    // Change pivot point to still be correct
    for (size_t i = 0; i < init.size(); i++)
        init[i] += scale * ranges[i].first;
    // Adjust cells and border cells
    auto adjust = [&](std::unordered_set<std::vector<int>, VectorHash> &set) ->
    void {
        std::unordered_set<std::vector<int>, VectorHash> setCopy = set;
        set.clear();
        for (std::vector<int> point : setCopy) {
            for (size_t i = 0; i < point.size(); i++)
                point[i] -= ranges[i].first;
            set.insert(point);
        }
    };
    adjust(cells);
    adjust(borderCells);
}

bool GridDomain::inDomain(const std::vector<int> &pos) const {
    std::vector<double> realPos;
    convertPos(pos, realPos);
    expr::ExprNode eq = expr;
    for (size_t i = 0; i < realPos.size(); i++) {
        expr::ExprNode search(expr::NODE_SYMB, {}, dims[i]);
        expr::ExprNode repl(expr::NODE_NUM, {}, std::to_string(realPos[i]));
        eq.replace(search, repl);
    }
    return eq.eval() != 0.0;
}

void GridDomain::convertPos(const std::vector<int> &pos, std::vector<double>
&out) const {
    out.clear();
    for (size_t i = 0; i < pos.size(); i++)
        out.push_back(double(pos[i]) * scale - init[i]);
}