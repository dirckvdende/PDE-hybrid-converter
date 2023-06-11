/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "expr/expr.h"
#include "pde/spec.h"
#include "preprocess.h"
#include <stack>
#include <unordered_map>

using namespace pde;
using namespace pde::parser;

Preprocessor::Preprocessor(PDESystem &system) : system(system) { }

Preprocessor::~Preprocessor() { }

void Preprocessor::run() {
    dbg::log("Running input preprocessor...");
    dimMap.clear();
    for (size_t i = 0; i < system.dims.size(); i++)
        dimMap.emplace(system.dims[i], i);
    replaceDims();
    replaceDerivs();
    dbg::log("Processed input:\n" + system.str());
}

void Preprocessor::replaceDims() {
    for (expr::ExprNode &node : system.init)
        node.replaceDims(dimMap);
    system.domain.replaceDims(dimMap);
    for (expr::ExprNode &node : system.vals)
        node.replaceDims(dimMap);
    for (expr::ExprNode &node : system.boundary)
        node.replaceDims(dimMap);
}

void Preprocessor::replaceDerivs() {
    for (expr::ExprNode &node : system.vals) {
        std::stack<expr::ExprNode *> nodes;
        nodes.push(&node);
        while (!nodes.empty()) {
            expr::ExprNode *node = nodes.top();
            nodes.pop();
            for (expr::ExprNode *child : node->children)
                nodes.push(child);
            if (node->type == expr::NODE_DERIV) {
                node->deriv.count = std::vector<size_t>(system.dims.size(), 0);
                for (const std::string &dim : node->deriv.dims)
                    node->deriv.count[dimMap[dim]]++;
            }
        }
    }
}