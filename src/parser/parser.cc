
#include "parser.h"
#include "treeparser/lexer.h"
#include "treeparser/parser.h"
#include <string>
#include <stdexcept>

Parser::Parser(const std::string &txt) : txt(txt) {

}

Parser::~Parser() { }

void Parser::run() {
    runTreeParser();
    // TODO: implement
}

void Parser::runTreeParser() {
    TreeLexer lexer(txt);
    lexer.run();
    TreeParser parser(lexer.getTokens());
    parser.run();
    TreeParseNode *root = parser.getRoot();
    if (root->children.size() != 1)
        invalidConfig();
    TreeParseNode *pde = root->children.front();
    if (pde->name != "pde")
        invalidConfig();
    const std::unordered_map<std::string, std::string *> refMap = {
        { "equation", &equation },
        { "boundary", &boundary },
        { "domain", &domain },
    };
    for (TreeParseNode *child : pde->children) {
        if (child->name == "pde")
            invalidConfig();
        *refMap.at(child->name) = child->text;
    }
}

void Parser::invalidConfig() {
    throw std::runtime_error("Invalid configuration");
}