
#include "expr/lexer.h"
#include "expr/parser.h"
#include "grid/domain.h"
#include <iostream>

void getExpr(const std::string &inp, ExprNode &node) {
    ExprLexer lexer(inp);
    lexer.run();
    ExprParser parser(lexer.getTokens());
    parser.run();
    node = parser.getTree();
}

int main() {
    ExprNode node(NODE_ERR);
    std::string inp = "x * x + y * y < 200";
    getExpr(inp, node);
    std::vector<std::string> dims = {"x", "y"};
    GridDomain domain(1.0, node, dims);
    std::vector<double> init = {0.0, 0.0};
    domain.findDomain(init);
    std::cout << domain.str();
    return 0;
}
