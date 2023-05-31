
#include "approx.h"
#include "pde/grid/generator/util.h"

using namespace pde;
using namespace pde::approx;

SpatialApprox::SpatialApprox(grid::Grid &grid) : grid(grid) { }

SpatialApprox::~SpatialApprox() { }

void SpatialApprox::convert(grid::GridCell &cell) {
    for (expr::ExprNode &node : cell.vals)
        replaceApprox(cell, node);
}

std::vector<double> SpatialApprox::getCoeffs(size_t n) {
    if (n % 2 == 0) {
        std::vector<double> coeffs(n + 1);
        // c_i = (-1)^(n-i) (n choose i) / scale^n
        double c = n % 2 == 0 ? 1.0 : -1.0;
        for (size_t i = 0; i < n; i++)
            c /= grid.scale;
        for (size_t i = 0; i <= n; i++) {
            coeffs[i] = c;
            c = c / (i + 1) * (n - i);
        }
        return coeffs;
    }
    std::vector<double> prev = getCoeffs(n - 1);
    std::vector<double> coeffs(n + 2, 0.0);
    for (size_t i = 0; i <= n + 1; i++) {
        if (i <= n - 1)
            coeffs[i] -= prev[i];
        if (i >= 2)
            coeffs[i] += prev[i - 2];
        coeffs[i] /= grid.scale;
    }
    return coeffs;
}

void SpatialApprox::replaceApprox(grid::GridCell &cell, expr::ExprNode &node) {
    if (node.type == expr::NODE_DERIV) {
        node = getApprox(cell, node);
        return;
    }
    for (expr::ExprNode *child : node.children)
        replaceApprox(cell, *child);
}

expr::ExprNode SpatialApprox::getApprox(grid::GridCell &cell, const
expr::ExprNode &node) {
    // TODO
    const std::vector<size_t> loc = grid.toLoc(cell);
    const SpatialApprox::Coeffs &coeffs = calcAllCoeffs(node.deriv.count);
    expr::ExprNode out(expr::NODE_ERR);
    for (const std::pair<std::vector<long>, double> &item : coeffs) {
        std::vector<size_t> pos(item.first.size());
        for (size_t i = 0; i < pos.size(); i++)
            pos[i] = size_t(loc[i] + item.first[i]);
        if (out.type == expr::NODE_ERR) {
            out = getCoeffExpr(cell, pos, item.second, node.deriv.var);
        } else {
            expr::ExprNode tmp = expr::ExprNode(expr::NODE_ADD);
            tmp[0] = out;
            tmp[1] = getCoeffExpr(cell, pos, item.second, node.deriv.var);
            out = tmp;
        }
    }
    return out;
}

const SpatialApprox::Coeffs &SpatialApprox::calcAllCoeffs(const
std::vector<size_t> &deriv) {
    if (coeffCache.find(deriv) == coeffCache.end()) {
        Coeffs out = {{std::vector<long>(deriv.size(), 0), 1.0}};
        for (size_t i = 0; i < deriv.size(); i++) {
            std::vector<double> cur = getCoeffs(deriv[i]);
            Coeffs rep;
            for (size_t j = 0; j < cur.size(); j++) {
                for (std::pair<std::vector<long>, double> item : out) {
                    item.second *= cur[j];
                    item.first[i] += j;
                    item.first[i] -= cur.size() / 2;
                    rep.push_back(item);
                }
            }
            out = rep;
        }
        coeffCache.emplace(deriv, out);
    }
    return coeffCache.at(deriv);
}

expr::ExprNode SpatialApprox::getCoeffExpr(const grid::GridCell &cell, const
std::vector<size_t> &pos, double coeff, const std::string &var) const {
    return expr::ExprNode(expr::NODE_MUL, {
        new expr::ExprNode(expr::NODE_NUM, {}, coeff),
        new expr::ExprNode(expr::NODE_SYMB, {}, grid::generator::toGridVar(var,
        pos, cell.group == grid[pos].group ? grid.iteration : grid.iteration - 1
        ))
    });
}