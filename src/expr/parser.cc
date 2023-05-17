
#include "parser.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace expr;

Parser::Parser() : tokens(lexer.getTokens()),
tree(ExprNode(NODE_ERR)) { }

Parser::~Parser() { }

void Parser::run() {
    lexer.run();
    pos = 0;
    ExprNode *expr = readAndOr();
    tree = *expr;
    delete expr;
}

void Parser::setText(const std::string &txt) {
    lexer.clearText();
    lexer.append(txt);
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
    expect({TOK_NUM, TOK_SYMB, TOK_DERIV, TOK_LBRACE, TOK_INTEG, TOK_LBRACKET});
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
    } else if (accept({TOK_INTEG})) {
        return readInteg();
    } else if (accept({TOK_LBRACKET})) {
        return readList();
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

ExprNode *Parser::readInteg() {
    expect({TOK_INTEG});
    next();
    expect({TOK_LBRACE});
    next();
    ExprNode *node = new ExprNode(NODE_INTEG);
    node->children.push_back(readAndOr());
    expect({TOK_COMMA});
    next();
    node->children.push_back(readAndOr());
    expect({TOK_RBRACE});
    next();
    return node;
}

ExprNode *Parser::readList() {
    expect({TOK_LBRACKET});
    next();
    ExprNode *node = new ExprNode(NODE_LIST);
    node->children.push_back(readAndOr());
    while (accept({TOK_COMMA})) {
        next();
        node->children.push_back(readAndOr());
    }
    expect({TOK_RBRACKET});
    next();
    return node;
}
