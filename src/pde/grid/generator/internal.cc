
#include "dbg/dbg.h"
#include "internal.h"
#include "util.h"
#include <stack>

using namespace pde::grid::generator;

InternalExprGenerator::~InternalExprGenerator() { }

void InternalExprGenerator::generate(GridCell &cell) {
    // TODO: implement approximation replacement
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.vals.clear();
    for (size_t i = 0; i < system.vars.size(); i++) {
        cell.vals.push_back(system.vals[i]);
        cell.vals.back().replaceDirect(loc);
    }
    replaceAll(cell);
    calcInit(cell);
}

void InternalExprGenerator::replaceAll(GridCell &cell) {
    genDimMap();
    for (expr::ExprNode &val : cell.vals) {
        std::stack<expr::ExprNode *> children;
        children.push(&val);
        while (!children.empty()) {
            expr::ExprNode *cur = children.top();
            children.pop();
            if (cur->type == expr::NODE_DERIV) {
                std::vector<size_t> amt(grid.dims.size(), 0);
                for (const std::string &dim : cur->deriv.dims)
                    amt[dimMap[dim]]++;
                *cur = generateApprox(cell, amt, cur->deriv.var);
            } else {
                for (expr::ExprNode *child : cur->children)
                    children.push(child);
            }
        }
    }
}

expr::ExprNode InternalExprGenerator::generateApprox(GridCell &cell, const
std::vector<size_t> &deriv, const std::string &name) {
    // TODO: implement properly
    (void)deriv;
    std::vector<size_t> mid = grid.toLoc(cell);
    std::vector<size_t> left = mid, right = mid;
    left[0]--;
    right[0]++;
    return expr::ExprNode(expr::NODE_ADD, {
        new expr::ExprNode(expr::NODE_ADD, {
            new expr::ExprNode(expr::NODE_MUL, {
                new expr::ExprNode(expr::NODE_NUM, {}, 1.0),
                new expr::ExprNode(expr::NODE_SYMB, {}, toGridVar(name, left, grid.iteration)),
            }),
            new expr::ExprNode(expr::NODE_MUL, {
                new expr::ExprNode(expr::NODE_NUM, {}, -2.0),
                new expr::ExprNode(expr::NODE_SYMB, {}, toGridVar(name, mid, grid.iteration)),
            })
        }),
        new expr::ExprNode(expr::NODE_MUL, {
            new expr::ExprNode(expr::NODE_NUM, {}, 1.0),
            new expr::ExprNode(expr::NODE_SYMB, {}, toGridVar(name, right, grid.iteration)),
        })
    });
}

void InternalExprGenerator::genDimMap() {
    if (!dimMap.empty())
        return;
    for (size_t i = 0; i < grid.dims.size(); i++)
        dimMap.emplace(grid.dims[i], i);
}

void InternalExprGenerator::calcInit(GridCell &cell) {
    std::vector<double> loc = grid.toRealLoc(cell);
    cell.init.clear();
    for (size_t i = 0; i < system.init.size(); i++) {
        cell.init.push_back(system.init[i].evalDirect(loc));
    }
}