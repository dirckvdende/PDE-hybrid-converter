
#include "expr.h"
#include "funcs.h"
#include <cmath>

using namespace expr;

const std::unordered_map<std::string, FuncDef> expr::funcs
= {
    {"sin", {
        [](double x) -> double { return std::sin(x); },
        [](const ExprNode &node) -> ExprNode * {
            return new ExprNode(NODE_MUL, {
                node[0].diffAlloc(),
                new ExprNode(NODE_FUNC, {
                    node[0].copy()
                }, "cos")
            });
        }
    }},
    {"cos", {
        [](double x) -> double { return std::cos(x); },
        [](const ExprNode &node) -> ExprNode * {
            return new ExprNode(NODE_MINUS, {
                new ExprNode(NODE_MUL, {
                    node[0].diffAlloc(),
                    new ExprNode(NODE_FUNC, {
                        node[0].copy()
                    }, "sin")
                })
            });
        }
    }},
    {"tan", {
        [](double x) -> double { return std::tan(x); },
        [](const ExprNode &node) -> ExprNode * {
            return new ExprNode(NODE_DIV, {
                node[0].diffAlloc(),
                new ExprNode(NODE_MUL, {
                    new ExprNode(NODE_FUNC, {
                        node[0].copy()
                    }, "cos"),
                    new ExprNode(NODE_FUNC, {
                        node[0].copy()
                    }, "cos"),
                })
            });
        }
    }},
    {"exp", {
        [](double x) -> double { return std::exp(x); },
        [](const ExprNode &node) -> ExprNode * {
            return new ExprNode(NODE_MUL, {
                node[0].diffAlloc(),
                new ExprNode(NODE_FUNC, {
                    node[0].copy()
                }, "exp")
            });
        }
    }},
    {"abs", {
        [](double x) -> double { return std::abs(x); },
        [](const ExprNode &node) -> ExprNode * {
            (void)node;
            return nullptr;
        }
    }},
};