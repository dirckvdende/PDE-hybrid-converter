
#include "eqparser.h"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

double EquationTree::eval(const std::unordered_map<std::string, double> &vals,
size_t root) const {
    root = std::min(root, size() - 1);
    const EquationNode &node = this->at(root);
    switch (node.type) {
        case NODE_ERR:
            return 0.0;
        case NODE_NUM:
            return std::stod(node.content);
        case NODE_SYMB:
            if (vals.find(node.content) == vals.end())
                throw std::runtime_error("Could not find value for "
                "substitution");
            return vals.at(node.content);
        case NODE_DERIV:
            throw std::runtime_error("Could not substitute derivative");
        case NODE_ADD:
            return eval(vals, node.children[0]) + eval(vals, node.children[1]);
        case NODE_SUB:
            return eval(vals, node.children[0]) - eval(vals, node.children[1]);
        case NODE_MUL:
            return eval(vals, node.children[0]) * eval(vals, node.children[1]);
        case NODE_DIV:
            return eval(vals, node.children[0]) / eval(vals, node.children[1]);
        case NODE_LT:
            return eval(vals, node.children[0]) < eval(vals, node.children[1]);
        case NODE_GT:
            return eval(vals, node.children[0]) > eval(vals, node.children[1]);
        case NODE_LTE:
            return eval(vals, node.children[0]) <= eval(vals, node.children[1]);
        case NODE_GTE:
            return eval(vals, node.children[0]) >= eval(vals, node.children[1]);
        case NODE_EQ:
            return eval(vals, node.children[0]) == eval(vals, node.children[1]);
    }
    return 0.0;
}

EquationParser::EquationParser(const std::vector<EquationToken> &tokens) : tokens(tokens) { }

EquationParser::~EquationParser() { }

void EquationParser::run() {
    readEquality();
}

const EquationTree &EquationParser::getTree() const {
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
    for (EquationTokenType type : types)
        if (cur().type == type)
            return true;
    return false;
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
