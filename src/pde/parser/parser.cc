
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "expr/parser.h"
#include "parser.h"
#include <stdexcept>

using namespace pde::parser;

Parser::Parser() { }

Parser::~Parser() { }

void Parser::setText(const std::string &txt) {
    this->txt = txt;
}

void Parser::run() {
    dbg::log("Running tree parser...");
    runTreeParser();
    dbg::log("Running expression parser...");
    runExprParser();
}

const PDESystem &Parser::getSpecs() const {
    return system;
}

void Parser::runTreeParser() {
    treeParser.setText(txt);
    static const treeparser::Settings settings = {
        {
            { "pde", treeparser::OPT_TREE },
            { "dims", treeparser::OPT_TEXT },
            { "domain", treeparser::OPT_TEXT },
            { "pivot", treeparser::OPT_TEXT },
            { "scale", treeparser::OPT_TEXT },
            { "equation", treeparser::OPT_TEXT },
            { "init", treeparser::OPT_TEXT },
            { "boundary", treeparser::OPT_TEXT },
            { "interval", treeparser::OPT_TEXT },
            { "time", treeparser::OPT_TEXT },
            { "iterations", treeparser::OPT_TEXT },
        }
    };
    treeParser.updateSettings(settings);
    treeParser.run();
    if (treeParser.getRoot()->children.size() != 1)
        throw std::runtime_error("Cannot handle multiple PDE systems");
    treeparser::ParseNode *root = treeParser.getRoot()->children.front();
    if (root->name != "pde")
        throw std::runtime_error("Missing \"pde\" root node");
    for (treeparser::ParseNode *child : root->children) {
        if (child->name == "pde")
            throw std::runtime_error("Invalid \"pde\" node location");
        if (preConfig.find(child->name) == preConfig.end())
            preConfig.emplace(child->name, std::vector<std::string>{});
        preConfig[child->name].push_back(child->text);
    }
}

void Parser::runExprParser() {
    checkRequiredFields();
    // TODO: implement
}

void Parser::checkRequiredFields() const {
    static const std::vector<std::string> required = {
        "dims", "domain", "pivot", "scale", "equation", "init", "boundary",
        "interval", "time", "iterations"
    };
    for (const std::string &req : required)
        if (preConfig.find(req) == preConfig.end())
            throw std::runtime_error("Missing input field \"" + req + "\"");
}

double Parser::parseNum(const std::string &txt) const {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    const expr::ExprNode &root = parser.getTree();
    return root.eval();
}