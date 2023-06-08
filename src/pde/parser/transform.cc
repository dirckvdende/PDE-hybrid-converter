
#include "dbg/dbg.h"
#include "transform.h"

using namespace pde::parser;

InputTransform::InputTransform(PreSystem &preSystem) :
preSystem(preSystem) { }

InputTransform::~InputTransform() { }

void InputTransform::run() {
    genEquations();
    genBoundary();
    genInitial();
    genBounds();
}

void InputTransform::genEquations() {
    std::vector<expr::ExprNode> eqs;
    for (expr::ExprNode &node : preSystem.equations) {
        std::string var = node[0].deriv.var;
        if (vars.find(var) != vars.end())
            dbg::error("Duplicate variable \"" + var + "\"");
        size_t count = node[0].deriv.dims.size();
        vars.emplace(var, count);
        for (size_t i = 1; i < count; i++) {
            eqs.push_back(expr::ExprNode(expr::NODE_EQ, {
                new expr::ExprNode(expr::NODE_DERIV),
                new expr::ExprNode(expr::NODE_SYMB, {}, derivName(var, i))
            }));
            eqs.back()[0].deriv.dims = {"t"};
            eqs.back()[0].deriv.var = derivName(var, i - 1);
        }
        node[0].deriv.var = derivName(var, count - 1);
        node[0].deriv.dims = {"t"};
    }
    preSystem.equations.insert(preSystem.equations.end(), eqs.begin(),
    eqs.end());
}

void InputTransform::genBoundary() {
    if (preSystem.boundaries.size() != vars.size())
        dbg::error("Invalid number of boundary conditions");
    std::unordered_set<std::string> found;
    std::vector<expr::ExprNode> boundaries;
    for (expr::ExprNode &node : preSystem.boundaries) {
        std::string var = node[0].content;
        if (found.find(var) != found.end())
            dbg::error("Duplicate boundary condition given");
        found.insert(var);
        expr::ExprNode d = node[1];
        for (size_t i = 1; i < vars[var]; i++) {
            d = d.diff();
            boundaries.push_back(expr::ExprNode(expr::NODE_EQ, {
                d.copy()
            }, derivName(var, i)));
        }
    }
    preSystem.boundaries.insert(preSystem.equations.end(), boundaries.begin(),
    boundaries.end());
}

void InputTransform::genInitial() {
    checkAllDerivs(preSystem.inits);
}

void InputTransform::genBounds() {
    checkAllDerivs(preSystem.intervals);
}

void InputTransform::checkAllDerivs(std::vector<expr::ExprNode> &nodes) {
    std::unordered_map<std::string, std::vector<size_t>> found;
    for (expr::ExprNode &node : nodes) {
        std::string var;
        size_t count = 0;
        if (node[0].type == expr::NODE_DERIV) {
            var = node[0].deriv.var;
            count = node[0].deriv.dims.size();
        } else {
            var = node[0].content;
        }
        if (found.find(var) == found.end())
            found.emplace(var, std::vector<size_t>{});
        found[var].push_back(count);
        node[0].type = expr::NODE_SYMB;
        node[0].content = derivName(var, count);
    }
    for (auto var : found) {
        std::sort(var.second.begin(), var.second.end());
        if (var.second.size() != vars[var.first])
            dbg::error("Not all initial conditions/bounds were given "
            "(correctly)");
        for (size_t i = 0; i < var.second.size(); i++)
            if (var.second[i] != i)
                dbg::error("Not all initial conditions/bounds were given "
                "(correctly)");
    }
}

std::string InputTransform::derivName(const std::string &var, size_t n) {
    if (n == 0)
        return var;
    return var + "__" + std::to_string(n);
}