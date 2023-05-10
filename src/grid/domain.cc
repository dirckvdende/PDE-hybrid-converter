
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