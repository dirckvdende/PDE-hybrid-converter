
#include "expr/expr.h"
#include "expr/lexer.h"
#include "expr/parser.h"
#include "fields.h"
#include "parser.h"
#include "treeparser/lexer.h"
#include "treeparser/parser.h"
#include "util.h"
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>

Parser::Parser(const std::string &txt) : txt(txt) { }

Parser::~Parser() { }

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

const InputFields &Parser::getInputFields() const {
    return inputFields;
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
    double iterations;
    storeNumber(fieldValues["iterations"], iterations);
    inputFields.iterations = size_t(iterations + 0.001);
    storeNumber(fieldValues["time"], inputFields.time);
    storeExpr(fieldValues["init"], inputFields.init);
    storeExpr(fieldValues["boundary"], inputFields.boundary);
    storeNumber(fieldValues["scale"], inputFields.scale);
    std::vector<std::string> pivot;
    storeCommaList(fieldValues["pivot"], pivot);
    inputFields.pivot.clear();
    for (const std::string &p : pivot)   
        inputFields.pivot.push_back(std::stod(p));
    storeExpr(fieldValues["domain"], inputFields.domain);
    storeExpr(fieldValues["func"], inputFields.func);
    storeExpr(fieldValues["equation"], inputFields.equation);
    storeCommaList(fieldValues["dimensions"], inputFields.dimensions);
}

void Parser::storeExpr(const std::string &str, expr::ExprNode &out) {
    expr::Parser parser;
    parser.setText(str);
    parser.run();
    out = parser.getTree();
}

void Parser::storeNumber(const std::string &str, double &out) {
    expr::ExprNode expr;
    storeExpr(str, expr);
    out = expr.eval();
}

void Parser::storeCommaList(const std::string &str, std::vector<std::string>
&out) {
    out = splitString(str);
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