
#include "dbg/dbg.h"
#include "validator.h"
#include <string>
#include <unordered_set>

using namespace pde::parser;

InputValidator::InputValidator(const PreSystem &preSystem) :
preSystem(preSystem) { }

InputValidator::~InputValidator() { }

void InputValidator::run() {
    if (preSystem.scale <= 1.0e-4)
        dbg::error("Given scale too small: " + std::to_string(preSystem.scale));
    if (preSystem.time <= 1.0e-4)
        dbg::error("Given time too small: " + std::to_string(preSystem.scale));
    if (preSystem.iterations < 1)
        dbg::error("Given number of iterations too small: " +
        std::to_string(preSystem.scale));
    checkExpressionFormats();
    checkEmits();
}

void InputValidator::checkExpressionFormats() {
    for (const std::vector<expr::ExprNode> &entry : {preSystem.equations,
    preSystem.inits, preSystem.boundaries, preSystem.intervals}) {
        for (const expr::ExprNode &node : entry) {
            if (node.type != expr::NODE_EQ || (node[0].type != expr::NODE_DERIV
            && node[0].type != expr::NODE_SYMB) || (node.type ==
            expr::NODE_DERIV && !onlyTimeDeriv(node)))
                dbg::error("Invalid expression for entry: \"" + node.str() +
                "\"");
            // Check for disallowed nodes on the right
            static const std::unordered_set<expr::NodeType> allowedTypes = {
                expr::NODE_SYMB, expr::NODE_NUM, expr::NODE_DERIV,
                expr::NODE_ADD, expr::NODE_SUB, expr::NODE_MUL, expr::NODE_DIV,
                expr::NODE_MINUS, expr::NODE_LIST, expr::NODE_FUNC,
            };
            node[1].walk([&](const expr::ExprNode &child) -> void {
                if (allowedTypes.find(child.type) == allowedTypes.end())
                    dbg::error("Disallowed expression types in \"" + node.str()
                    + "\"");
            });
        }
    }
    for (const expr::ExprNode &node : preSystem.equations)
        if (node[0].type != expr::NODE_DERIV)
            dbg::error("Invalid expression for entry: \"" + node.str() + "\"");
}

void InputValidator::checkEmits() {
    // TODO: implement
}

bool InputValidator::onlyTimeDeriv(const expr::ExprNode &node) {
    for (const std::string &dim : node.deriv.dims)
        if (dim != "t")
            return false;
    return true;
}