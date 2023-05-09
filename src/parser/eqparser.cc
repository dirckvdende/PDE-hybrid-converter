
#include "eqparser.h"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

EquationParser::EquationParser(const std::vector<EquationToken> &tokens) : tokens(tokens) { }

EquationParser::~EquationParser() { }

void EquationParser::run() {
    readEquality();
}

const std::vector<EquationNode> &EquationParser::getTree() const {
    return tree;
}

EquationToken EquationParser::cur() const {
    return pos < tokens.size() ? tokens[pos] : EquationToken{EQTOK_ERR};
}

void EquationParser::next() {
    pos++;
}

bool EquationParser::atEnd() const {
    return pos >= tokens.size();
}

bool EquationParser::accept(std::vector<EquationTokenType> types) const {

}

void EquationParser::expect(std::vector<EquationTokenType> types) {
    if (!accept(types))
        throw std::runtime_error("Could not parse input");
}

size_t EquationParser::readEquality() {
    size_t left = readSum();
    if (!accept({EQTOK_LT, EQTOK_LTE, EQTOK_GT, EQTOK_GTE, EQTOK_EQ}))
        return left;
    EquationTokenType type = cur().type;
    next();
    size_t right = readSum();
    const std::unordered_map<EquationTokenType, EquationNodeType> typeMap = {
        { EQTOK_LT, NODE_LT },
        { EQTOK_GT, NODE_GT },
        { EQTOK_LTE, NODE_LTE },
        { EQTOK_GTE, NODE_GTE },
        { EQTOK_EQ, NODE_EQ },
    };
    tree.push_back({typeMap.at(type), "", {left, right}});
}

size_t EquationParser::readSum() {
    size_t left = readProduct();
    while (accept({EQTOK_ADD, EQTOK_SUB})) {
        EquationNodeType type = cur().type == EQTOK_ADD ? NODE_ADD : NODE_SUB;
        next();
        size_t right = readProduct();
        tree.push_back({type, "", {left, right}});
        left = tree.size() - 1;
    }
    return left;
}

size_t EquationParser::readProduct() {
    size_t left = readTerm();
    while (accept({EQTOK_MUL, EQTOK_DIV})) {
        EquationNodeType type = cur().type == EQTOK_MUL ? NODE_MUL : NODE_DIV;
        next();
        size_t right = readTerm();
        tree.push_back({type, "", {left, right}});
        left = tree.size() - 1;
    }
    return left;
}

size_t EquationParser::readTerm() {
    expect({EQTOK_NUM, EQTOK_SYMB, EQTOK_DERIV, EQTOK_LBRACE});
    if (accept({EQTOK_NUM})) {
        tree.push_back({NODE_NUM, cur().content});
        next();
        return tree.size() - 1;
    } else if (accept({EQTOK_SYMB})) {
        tree.push_back({NODE_SYMB, cur().content});
        next();
        return tree.size() - 1;
    } else if (accept({EQTOK_DERIV})) {
        return readDeriv();
    } else if (accept({EQTOK_LBRACE})) {
        next();
        size_t result = readEquality();
        next();
        return result;
    }
    return SIZE_MAX;
}

size_t EquationParser::readDeriv() {
    expect({EQTOK_DERIV});
    std::string content = cur().content.substr(1);
    if (content.size() == 0)
        throw std::runtime_error("Invalid derivative");
    next();
    expect({EQTOK_LBRACE});
    next();
    size_t child = readEquality();
    expect({EQTOK_RBRACE});
    next();
    tree.push_back({NODE_DERIV, content, {child}});
}
