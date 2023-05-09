
#include "eqlexer.h"
#include "eqparser.h"
#include "parser.h"
#include "treeparser/lexer.h"
#include "treeparser/parser.h"
#include <string>
#include <stdexcept>
#include <vector>

Parser::Parser(const std::string &txt) : txt(txt) { }

Parser::~Parser() { }

void Parser::run() {
    runTreeParser();
    lexEquations();
    parseEquations();
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

void Parser::lexEquations() {
    std::vector<std::pair<std::string *, std::vector<EquationToken> *>>
    toConvert = {
        { &equation, &equationTokens },
        { &boundary, &boundaryTokens },
        { &domain, &domainTokens },
    };
    for (const auto &item : toConvert) {
        EquationLexer lexer(*item.first);
        lexer.run();
        *item.second = lexer.getTokens();
    }
}

void Parser::parseEquations() {
    std::vector<std::pair<std::vector<EquationToken> *,
    std::vector<EquationNode> *>> toConvert = {
        { &equationTokens, &equationTree },
        { &boundaryTokens, &boundaryTree },
        { &domainTokens, &domainTree },
    };
    for (const auto &item : toConvert) {
        EquationParser parser(*item.first);
        parser.run();
        *item.second = parser.getTree();
    }
}

void Parser::invalidConfig() {
    throw std::runtime_error("Invalid configuration");
}