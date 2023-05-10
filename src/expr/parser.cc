
#include "parser.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

ExprParser::ExprParser(const std::vector<ExprToken> &tokens) : tokens(tokens),
tree(ExprNode(NODE_ERR)) { }

ExprParser::~ExprParser() { }

void ExprParser::run() {
    pos = 0;
    ExprNode *expr = readEquality();
    tree = *expr;
    delete expr;
}

const ExprNode &ExprParser::getTree() const {
    return tree;
}

ExprToken ExprParser::cur() const {
    return pos < tokens.size() ? tokens[pos] : ExprToken{EXPRTOK_ERR};
}

void ExprParser::next() {
    pos++;
}

bool ExprParser::atEnd() const {
    return pos >= tokens.size();
}

bool ExprParser::accept(std::vector<ExprTokenType> types) const {
    for (ExprTokenType type : types)
        if (cur().type == type)
            return true;
    return false;
}

void ExprParser::expect(std::vector<ExprTokenType> types) {
    if (!accept(types)) {
        std::sort(types.begin(), types.end());
        std::string typeList = std::to_string(types.front());
        for (size_t i = 1; i < types.size(); i++)
            typeList.append(", " + std::to_string(types[i]));
        throw std::runtime_error("Could not parse input, expected (" + typeList
        + ") at position " + std::to_string(pos));
    }
}

ExprNode *ExprParser::readEquality() {
    ExprNode *left = readSum();
    if (!accept({EXPRTOK_LT, EXPRTOK_LTE, EXPRTOK_GT, EXPRTOK_GTE, EXPRTOK_EQ}))
        return left;
    ExprTokenType type = cur().type;
    next();
    ExprNode *right = readSum();
    const std::unordered_map<ExprTokenType, ExprNodeType> typeMap = {
        { EXPRTOK_LT, NODE_LT },
        { EXPRTOK_GT, NODE_GT },
        { EXPRTOK_LTE, NODE_LTE },
        { EXPRTOK_GTE, NODE_GTE },
        { EXPRTOK_EQ, NODE_EQ },
    };
    return new ExprNode(typeMap.at(type), {left, right});
}

ExprNode *ExprParser::readSum() {
    ExprNode *left = readProduct();
    while (accept({EXPRTOK_ADD, EXPRTOK_SUB})) {
        ExprNodeType type = cur().type == EXPRTOK_ADD ? NODE_ADD : NODE_SUB;
        next();
        ExprNode *right = readProduct();
        left = new ExprNode(type, {left, right});
    }
    return left;
}

ExprNode *ExprParser::readProduct() {
    ExprNode *left = readTerm();
    while (accept({EXPRTOK_MUL, EXPRTOK_DIV})) {
        ExprNodeType type = cur().type == EXPRTOK_MUL ? NODE_MUL : NODE_DIV;
        next();
        ExprNode *right = readTerm();
        left = new ExprNode(type, {left, right});
    }
    return left;
}

ExprNode *ExprParser::readTerm() {
    expect({EXPRTOK_NUM, EXPRTOK_SYMB, EXPRTOK_DERIV, EXPRTOK_LBRACE});
    if (accept({EXPRTOK_NUM})) {
        ExprNode *node = new ExprNode(NODE_NUM, {}, cur().content);
        next();
        return node;
    } else if (accept({EXPRTOK_SYMB})) {
        ExprNode *node = new ExprNode(NODE_SYMB, {}, cur().content);
        next();
        return node;
    } else if (accept({EXPRTOK_DERIV})) {
        return readDeriv();
    } else if (accept({EXPRTOK_LBRACE})) {
        next();
        ExprNode *result = readEquality();
        next();
        return result;
    }
    return nullptr;
}

ExprNode *ExprParser::readDeriv() {
    expect({EXPRTOK_DERIV});
    std::string content = cur().content.substr(1);
    if (content.size() == 0)
        throw std::runtime_error("Invalid derivative");
    next();
    expect({EXPRTOK_LBRACE});
    next();
    expect({EXPRTOK_DERIV, EXPRTOK_SYMB});
    ExprNode *node = nullptr;
    if (accept({EXPRTOK_DERIV})) {
        node = readDeriv();
        node->content = content + " " + node->content;
    } else {
        node = new ExprNode(NODE_DERIV, {}, content + ";" + cur().content);
        next();
    }
    expect({EXPRTOK_RBRACE});
    next();
    return node;
}
