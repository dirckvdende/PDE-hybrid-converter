
#include <iostream>
#include <vector>
#include "lexer.h"
#include "parser.h"

using namespace treeparser;

ParseNode::ParseNode(const ParseNodeType type) : type(type) { }

ParseNode::~ParseNode() {
    for (ParseNode *child : children)
        delete child;
}

std::ostream &ParseNode::print(std::ostream &os, size_t depth) {
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
    for (ParseNode *child : children)
        child->print(os, depth + 1);
    return os;
}

Parser::Parser() : root(nullptr), curIndex(0), tokens(lexer.getTokens())
{ }

Parser::~Parser() {
    delete root;
}

void Parser::setText(std::string txt) {
    lexer.clearText();
    lexer.append(txt);
}

void Parser::run() {
    prepareConfigOptions();
    lexer.run();
    root = readRoot();
}

void Parser::updateSettings(const Settings &sett) {
    lexer.updateSettings(sett);
    settings = sett;
}

ParseNode *Parser::getRoot() const {
    return root;
}

const Token &Parser::cur() const {
    if (curIndex >= tokens.size())
        return tokens.back();
    return tokens[curIndex];
}

void Parser::next() {
    curIndex++;
}

bool Parser::atEnd() const {
    return curIndex >= tokens.size();
}

bool Parser::accept(TokenType type) const {
    if (atEnd())
        return false;
    return cur().type == type;
}

void Parser::expect(TokenType type) {
    if (!accept(type))
        error("Unexpected token of type (" + std::to_string(cur().type) +
        ") with content \"" + cur().content + "\"");
}

void Parser::error(std::string content) {
    std::cerr << "ERROR: " << content << std::endl;
    exit(1);
}

ParseNode *Parser::readRoot() {
    ParseNode *node = new ParseNode(NODE_ROOT);
    while (!atEnd())
        node->children.push_back(readConfigEntry());
    return node;
}

ParseNode *Parser::readConfigEntry() {
    expect(TOK_NAME);
    const ConfigOption &option = *configNames[cur().content];
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

ParseNode *Parser::readTextEntry() {
    expect(TOK_NAME);
    ParseNode *node = new ParseNode(NODE_TEXT);
    node->name = cur().content;
    next();
    expect(TOK_TEXT);
    node->text = cur().content;
    next();
    expect(TOK_SEMICOL);
    next();
    return node;
}

ParseNode *Parser::readTreeEntry(bool isNamed) {
    expect(TOK_NAME);
    ParseNode *node = new ParseNode(isNamed ? NODE_NAMED_TREE : NODE_TREE);
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

void Parser::prepareConfigOptions() {
    configNames.clear();
    for (const ConfigOption &option : settings.configOptions)
        configNames.emplace(option.name, &option);
}
