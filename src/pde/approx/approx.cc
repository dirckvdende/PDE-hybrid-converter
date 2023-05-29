
#include "approx.h"
#include "pde/grid/generator/util.h"

using namespace pde;
using namespace pde::approx;

SpatialApprox::SpatialApprox() : grid(nullptr), curCell(nullptr), iteration(0)
{ }

SpatialApprox::~SpatialApprox() { }

void SpatialApprox::setScale(double val) {
    scale = val;
}

void SpatialApprox::setDims(const std::vector<std::string> &dims) {
    dimMap.clear();
    for (size_t i = 0; i < dims.size(); i++)
        dimMap.emplace(dims[i], i);
}

void SpatialApprox::setGrid(grid::Grid &gridRef) {
    grid = &gridRef;
}

void SpatialApprox::setIteration(size_t val) {
    iteration = val;
}

void SpatialApprox::convert() {
    for (grid::GridCell &cell : *grid)
        convert(cell);
}

void SpatialApprox::convert(grid::GridCell &cell) {
    curCell = &cell;
    for (expr::ExprNode &node : cell.vals)
        replaceApprox(node);
    curCell = nullptr;
}

std::vector<double> SpatialApprox::getCoeffs(size_t n) {
    if (n % 2 == 0) {
        std::vector<double> coeffs(n + 1);
        // c_i = (-1)^(n-i) (n choose i) / scale^n
        double c = 1.0;
        if (n % 2 != 0)
            c = -1.0;
        for (size_t i = 0; i < n; i++)
            c /= scale;
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
        coeffs[i] /= scale;
    }
    return coeffs;
}

void SpatialApprox::replaceApprox(expr::ExprNode &node) const {
    if (node.type == expr::NODE_DERIV) {
        node = getApprox(node);
        return;
    }
    for (expr::ExprNode *child : node.children)
        replaceApprox(*child);
}

expr::ExprNode SpatialApprox::getApprox(const expr::ExprNode &node) const {
    // TODO: implement
    if (node.deriv.dims != std::vector<std::string>{"x", "x"})
        throw std::runtime_error("not implemented");
    std::vector<size_t> cur = grid->toLoc(curCell);
    cur[0]--;
    std::vector<size_t> left = cur;
    cur[0]++;
    std::vector<size_t> mid = cur;
    cur[0]++;
    std::vector<size_t> right = cur;
    expr::ExprNode out(expr::NODE_ADD, {
        new expr::ExprNode(expr::NODE_ADD, {
            new expr::ExprNode(expr::NODE_MUL, {
                new expr::ExprNode(expr::NODE_NUM, {}, 1.0 / scale / scale),
                new expr::ExprNode(expr::NODE_SYMB, {}, grid::generator::toGridVar(node.deriv.var, left, iteration))
            }),
            new expr::ExprNode(expr::NODE_MUL, {
                new expr::ExprNode(expr::NODE_NUM, {}, -2.0 / scale / scale),
                new expr::ExprNode(expr::NODE_SYMB, {}, grid::generator::toGridVar(node.deriv.var, mid, iteration))
            })
        }),
        new expr::ExprNode(expr::NODE_MUL, {
            new expr::ExprNode(expr::NODE_NUM, {}, 1.0 / scale / scale),
            new expr::ExprNode(expr::NODE_SYMB, {}, grid::generator::toGridVar(node.deriv.var, right, iteration))
        })
    });
    return out;
}