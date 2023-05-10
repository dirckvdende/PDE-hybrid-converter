
#include "domain.h"
#include "expr/expr.h"
#include <unordered_map>
#include <vector>
#include <queue>

GridDomain::GridDomain(double scale, const ExprNode &expr, const
std::vector<std::string> &dims) : scale(scale), expr(expr), dims(dims) { }

void GridDomain::findDomain(const std::vector<double> &init) {
    this->init = init;
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
                if (inDomain(e)) {
                    if (cells.size() >= maxSize)
                        throw std::runtime_error("Exceeded maximum grid size");
                    q.push(e);
                    cells.insert(e);
                }
            }
        }
    }
}

std::string GridDomain::str() const {
    if (dims.size() != 2)
        throw std::runtime_error("Cannot convert non-2D grid to string");
    std::vector<std::pair<int, int>> ranges(dims.size(), {0, 0});
    for (const std::vector<int> &cell : cells) {
        for (size_t i = 0; i < cell.size(); i++) {
            if (cell[i] < ranges[i].first)
                ranges[i].first = cell[i];
            if (cell[i] > ranges[i].second)
                ranges[i].second = cell[i];
        }
    }
    std::string out;
    for (int x = ranges[0].first; x <= ranges[0].second; x++) {
        for (int y = ranges[1].first; y <= ranges[1].second; y++)
            out.push_back(cells.find({x, y}) != cells.end() ? '1' : '0');
        out.push_back('\n');
    }
    return out;
}

bool GridDomain::inDomain(const std::vector<int> &pos) const {
    std::vector<double> realPos;
    convertPos(pos, realPos);
    ExprNode eq = expr;
    for (size_t i = 0; realPos.size(); i++) {
        ExprNode search(NODE_SYMB, {}, dims[i]);
        ExprNode repl(NODE_NUM, {}, std::to_string(realPos[i]));
        eq.replace(search, repl);
    }
    return eq.eval() != 0.0;
}

void GridDomain::convertPos(const std::vector<int> &pos, std::vector<double>
&out) const {
    out.clear();
    out.reserve(out.size());
    for (size_t i = 0; i < pos.size(); i++)
        out.push_back(double(pos[i]) * scale - init[i]);
}