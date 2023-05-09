
#include <iostream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "settings.h"

TreeParseNode::TreeParseNode(const TreeParseNodeType type) : type(type) { }

TreeParseNode::~TreeParseNode() {
    for (TreeParseNode *child : children)
        delete child;
}

std::ostream &TreeParseNode::print(std::ostream &os, size_t depth) {
    for (size_t i = 0; i < depth; i++)
        os << "  ";
    switch (type) {
        case NODE_TEXT:
            os << "TEXT  ";
            break;
        case NODE_TREE:
            os << "TREE  ";
            break;
        case NODE_NAMED_TREE:
            os << "NTREE ";
            break;
        case NODE_ROOT:
            os << "ROOT  ";
            break;
    }
    os << name << "; " << text << '\n';
    for (TreeParseNode *child : children)
        child->print(os, depth + 1);
    return os;
}

TreeParser::TreeParser(const std::vector<Token> &tokens) : tokens(tokens),
root(nullptr), curIndex(0) {
    prepareConfigOptions();
}

TreeParser::~TreeParser() {
    delete root;
}

void TreeParser::run() {
    root = readRoot();
}

std::ostream &operator<<(std::ostream &os, const TreeParser &parser) {
    if (parser.root == nullptr)
        return os;
    return parser.root->print(os, 0);
}

const Token &TreeParser::cur() const {
    if (curIndex >= tokens.size())
        return tokens.back();
    return tokens[curIndex];
}

void TreeParser::next() {
    curIndex++;
}

bool TreeParser::atEnd() const {
    return curIndex >= tokens.size();
}

bool TreeParser::accept(TokenType type) const {
    if (atEnd())
        return false;
    return cur().type == type;
}

void TreeParser::expect(TokenType type) {
    if (!accept(type))
        error("Unexpected token");
}

void TreeParser::error(std::string content) {
    std::cerr << "ERROR: " << content << std::endl;
    exit(1);
}

TreeParseNode *TreeParser::readRoot() {
    TreeParseNode *node = new TreeParseNode(NODE_ROOT);
    while (!atEnd())
        node->children.push_back(readConfigEntry());
    return node;
}

TreeParseNode *TreeParser::readConfigEntry() {
    expect(TOK_NAME);
    const TreeConfigOption &option = *configNames[cur().content];
    switch (option.type) {
        case OPT_TEXT:
            return readTextEntry();
        case OPT_TREE:
            return readTreeEntry(false);
        case OPT_NAMED_TREE:
            return readTreeEntry(true);
    }
    return nullptr;
}

TreeParseNode *TreeParser::readTextEntry() {
    expect(TOK_NAME);
    TreeParseNode *node = new TreeParseNode(NODE_TEXT);
    node->name = cur().content;
    next();
    expect(TOK_TEXT);
    node->text = cur().content;
    next();
    expect(TOK_SEMICOL);
    next();
    return node;
}

TreeParseNode *TreeParser::readTreeEntry(bool isNamed) {
    expect(TOK_NAME);
    TreeParseNode *node = new TreeParseNode(isNamed ? NODE_NAMED_TREE : NODE_TREE);
    node->name = cur().content;
    next();
    if (isNamed) {
        expect(TOK_TEXT);
        node->text = cur().content;
        next();
    }
    expect(TOK_LBRACE);
    next();
    while (!accept(TOK_RBRACE))
        node->children.push_back(readConfigEntry());
    expect(TOK_RBRACE);
    next();
    // Ignore semicolon after brace
    if (accept(TOK_SEMICOL))
        next();
    return node;
}

void TreeParser::prepareConfigOptions() {
    for (const TreeConfigOption &option : settings.configOptions)
        configNames.emplace(option.name, &option);
}
