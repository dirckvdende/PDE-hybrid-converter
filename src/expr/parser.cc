
#include "parser.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace expr;

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens),
tree(ExprNode(NODE_ERR)) { }

Parser::~Parser() { }

void Parser::run() {
    pos = 0;
    ExprNode *expr = readAndOr();
    tree = *expr;
    delete expr;
}

const ExprNode &Parser::getTree() const {
    return tree;
}

Token Parser::cur() const {
    return pos < tokens.size() ? tokens[pos] : Token{TOK_ERR};
}

void Parser::next() {
    pos++;
}

bool Parser::atEnd() const {
    return pos >= tokens.size();
}

bool Parser::accept(std::vector<TokenType> types) const {
    for (TokenType type : types)
        if (cur().type == type)
            return true;
    return false;
}

void Parser::expect(std::vector<TokenType> types) {
    if (!accept(types)) {
        std::sort(types.begin(), types.end());
        std::string typeList = std::to_string(types.front());
        for (size_t i = 1; i < types.size(); i++)
            typeList.append(", " + std::to_string(types[i]));
        throw std::runtime_error("Could not parse input, expected (" + typeList
        + ") at position " + std::to_string(pos));
    }
}

ExprNode *Parser::readAndOr() {
    ExprNode *left = readEquality();
    while (accept({TOK_AND, TOK_OR})) {
        NodeType type = cur().type == TOK_AND ? NODE_AND : NODE_OR;
        next();
        ExprNode *right = readEquality();
        left = new ExprNode(type, {left, right});
    }
    return left;
}

ExprNode *Parser::readEquality() {
    ExprNode *left = readSum();
    if (!accept({TOK_LT, TOK_LTE, TOK_GT, TOK_GTE, TOK_EQ}))
        return left;
    TokenType type = cur().type;
    next();
    ExprNode *right = readSum();
    const std::unordered_map<TokenType, NodeType> typeMap = {
        { TOK_LT, NODE_LT },
        { TOK_GT, NODE_GT },
        { TOK_LTE, NODE_LTE },
        { TOK_GTE, NODE_GTE },
        { TOK_EQ, NODE_EQ },
    };
    return new ExprNode(typeMap.at(type), {left, right});
}

ExprNode *Parser::readSum() {
    ExprNode *left = readProduct();
    while (accept({TOK_ADD, TOK_SUB})) {
        NodeType type = cur().type == TOK_ADD ? NODE_ADD : NODE_SUB;
        next();
        ExprNode *right = readProduct();
        left = new ExprNode(type, {left, right});
    }
    return left;
}

ExprNode *Parser::readProduct() {
    ExprNode *left = readMinus();
    while (accept({TOK_MUL, TOK_DIV})) {
        NodeType type = cur().type == TOK_MUL ? NODE_MUL : NODE_DIV;
        next();
        ExprNode *right = readMinus();
        left = new ExprNode(type, {left, right});
    }
    return left;
}

ExprNode *Parser::readMinus() {
    if (!accept({TOK_SUB}))
        return readTerm();
    next();
    return new ExprNode(NODE_MINUS, {readTerm()});
}

ExprNode *Parser::readTerm() {
    expect({TOK_NUM, TOK_SYMB, TOK_DERIV, TOK_LBRACE});
    if (accept({TOK_NUM})) {
        ExprNode *node = new ExprNode(NODE_NUM, {}, cur().content);
        next();
        return node;
    } else if (accept({TOK_SYMB})) {
        ExprNode *node = new ExprNode(NODE_SYMB, {}, cur().content);
        next();
        return node;
    } else if (accept({TOK_DERIV})) {
        return readDeriv();
    } else if (accept({TOK_LBRACE})) {
        next();
        ExprNode *result = readAndOr();
        next();
        return result;
    }
    return nullptr;
}

ExprNode *Parser::readDeriv() {
    expect({TOK_DERIV});
    std::string content = cur().content.substr(1);
    if (content.size() == 0)
        throw std::runtime_error("Invalid derivative");
    next();
    expect({TOK_LBRACE});
    next();
    expect({TOK_DERIV, TOK_SYMB});
    ExprNode *node = nullptr;
    if (accept({TOK_DERIV})) {
        node = readDeriv();
        node->content = content + " " + node->content;
    } else {
        node = new ExprNode(NODE_DERIV, {}, content + ";" + cur().content);
        next();
    }
    expect({TOK_RBRACE});
    next();
    return node;
}
