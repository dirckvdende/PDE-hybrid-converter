
#include "expr/expr.h"
#include <iostream>

int main() {
    ExprNode a(NODE_NUM);
    a.content = "5";
    ExprNode b = a;
    b.content = "7";
    ExprNode sm(NODE_ADD);
    sm[0] = a, sm[1] = b;
    std::cout << sm.str() << std::endl;
    return 0;
}
