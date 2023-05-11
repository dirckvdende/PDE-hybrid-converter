
#include "expr/expr.h"
#include "expr/lexer.h"
#include "expr/parser.h"
#include "parser.h"
#include "treeparser/lexer.h"
#include "treeparser/parser.h"
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>

Parser::Parser(const std::string &txt) : txt(txt) { }

Parser::~Parser() {
    for (const std::pair<std::string, ExprNode *> &field : fieldExpr)
        delete field.second;
}

void Parser::run() {
    runTreeParser();
    if (!hasAllFields())
        throw std::runtime_error("Not all required configuration options were "
        "given");
    parseExpressions();
}

const std::unordered_map<std::string, std::string> &Parser::getFieldValues()
const {
    return fieldValues;
}

const std::unordered_map<std::string, ExprNode *> &Parser::getFieldExpressions()
const {
    return fieldExpr;
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
    for (TreeParseNode *child : pde->children) {
        if (child->name == "pde")
            invalidConfig();
        fieldValues[child->name] = child->text;
    }
}

void Parser::parseExpressions() {
    static const std::vector<std::string> exprFields = {
        "equation", "func", "domain", "scale", "boundary", "init", "time",
        "iterations",
    };
    for (const std::string &field : exprFields) {
        ExprLexer lexer(fieldValues[field]);
        lexer.run();
        ExprParser parser(lexer.getTokens());
        parser.run();
        ExprNode *expr = new ExprNode(NODE_ERR);
        *expr = parser.getTree();
        fieldExpr.emplace(field, expr);
    }
}

bool Parser::hasAllFields() const {
    static const std::vector<std::string> requiredFields = {
        "dimensions", "equation", "func", "domain", "pivot", "scale",
        "boundary", "init", "time", "iterations",
    };
    for (const std::string &req : requiredFields)
        if (fieldValues.find(req) == fieldValues.end())
            return false;
    return true;
}

void Parser::invalidConfig() {
    throw std::runtime_error("Invalid configuration");
}