
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "expr/parser.h"
#include "parser.h"
#include "treeparser/settings.h"
#include <algorithm>
#include <stdexcept>

using namespace sim;

Parser::Parser() { }

Parser::~Parser() { }

void Parser::setText(const std::string &txt) {
    this->txt = txt;
    treeParser.setText(txt);
}

void Parser::run() {
    dbg::log("Running tree parser...");
    runTreeParser();
    dbg::log("Running expression parser...");
    runExprParser();
}

const std::vector<ODESystem> &Parser::getSpecs() const {
    return specs;
}

void Parser::runTreeParser() {
    treeParser.setText(txt);
    static const treeparser::Settings settings = {
        {
            { "system", treeparser::OPT_TREE },
            { "var", treeparser::OPT_TEXT },
            { "interval", treeparser::OPT_TEXT },
            { "emit", treeparser::OPT_TEXT },
            { "time", treeparser::OPT_TEXT },
        }
    };
    treeParser.updateSettings(settings);
    treeParser.run();
    for (treeparser::ParseNode *node : treeParser.getRoot()->children) {
        if (node->name != "system")
            throw std::runtime_error("Missing \"system\" root node");
        preConfig.emplace_back();
        for (treeparser::ParseNode *child : node->children) {
            if (child->name == "system")
                throw std::runtime_error("Invalid \"system\" node location");
            preConfig.back().entries.push_back({child->name, child->text});
        }
    }
}

void Parser::runExprParser() {
    for (const ODEPreConfig &conf : preConfig) {
        specs.emplace_back();
        ODESystem &system = specs.back();
        for (const std::pair<std::string, std::string> &entry : conf.entries) {
            if (entry.first == "var")
                parseVar(entry.second, system);
            if (entry.first == "interval")
                parseInterval(entry.second, system);
            if (entry.first == "emit")
                parseEmit(entry.second, system);
            if (entry.first == "time")
                parseTime(entry.second, system);
        }
    }
}

void Parser::parseVar(const std::string &txt, ODESystem &system) {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    const expr::ExprNode &root = parser.getTree();
    if (root.type != expr::NODE_EQ)
        throw std::runtime_error("Invalid expression after \"var\"");
    if (root[0].type != expr::NODE_SYMB)
        throw std::runtime_error("Invalid expression after \"var\"");
    system.vars.push_back(root[0].content);
    system.vals.push_back(root[1]);
    // Default bounds to -1.0, 1.0
    system.bounds.push_back({-1.0, 1.0});
}

void Parser::parseInterval(const std::string &txt, ODESystem &system) {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    const expr::ExprNode &root = parser.getTree();
    if (root.type != expr::NODE_EQ)
        throw std::runtime_error("Invalid expression after \"interval\"");
    if (root[0].type != expr::NODE_SYMB)
        throw std::runtime_error("Invalid expression after \"interval\"");
    size_t index = system.vars.size();
    for (size_t i = 0; i < system.vars.size(); i++)
        if (system.vars[i] == root[0].content)
            index = i;
    if (index == system.vars.size() || root[1].type != expr::NODE_LIST ||
    root[1].children.size() != 2)
        throw std::runtime_error("Invalid expression after \"interval\"");
    system.bounds[index] = {root[1][0].eval(), root[1][1].eval()};
}

void Parser::parseEmit(const std::string &txt, ODESystem &system) {
    size_t a = txt.find(' ');
    if (a >= txt.size())
        throw std::runtime_error("Invalid emit syntax");
    size_t b = txt.find(' ', a + 1);
    if (b >= txt.size())
        throw std::runtime_error("Invalid emit syntax");
    std::string first = txt.substr(0, a);
    std::string second = txt.substr(a + 1, b - a - 1);
    std::string third = txt.substr(b + 1);
    if (second != "as")
        throw std::runtime_error("Invalid emit syntax");
    system.emit.push_back({first, third});
}

void Parser::parseTime(const std::string &txt, ODESystem &system) {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    system.time = parser.getTree().eval();
}