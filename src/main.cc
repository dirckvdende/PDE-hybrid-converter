
#include "expr/lexer.h"
#include "expr/parser.h"
#include <iostream>

int main() {
    std::string inp = "5 + 7 * 8";
    ExprLexer lexer(inp);
    lexer.run();
    ExprParser parser(lexer.getTokens());
    parser.run();
    std::cout << parser.getTree().str() << std::endl;
    return 0;
}
