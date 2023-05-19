
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "expr/parser.h"
#include "parser.h"
#include <algorithm>
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
    dbg::log("Done parsing expressions:");
    dbg::log(system.str());
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
    parseDims(preConfig.at("dims").front());
    system.domain = parseExpr(preConfig.at("domain").front());
    system.pivot = parseNumList(preConfig.at("pivot").front());
    system.scale = parseNum(preConfig["scale"].front());
    for (const std::string &entry : preConfig.at("equation"))
        parseEquation(entry);
    for (const std::string &entry : preConfig.at("init"))
        parseAssignmentExpr(entry, system.init);
    for (const std::string &entry : preConfig.at("boundary"))
        parseAssignmentExpr(entry, system.boundary);
    for (const std::string &entry : preConfig.at("interval"))
        parseInterval(entry);
    system.time = parseNum(preConfig["time"].front());
    system.iterations = size_t(0.5 + parseNum(preConfig["iterations"].front()));
}

void Parser::checkRequiredFields() const {
    // Check all required fields
    static const std::vector<std::string> required = {
        "dims", "domain", "pivot", "scale", "equation", "init", "boundary",
        "interval", "time", "iterations"
    };
    for (const std::string &req : required)
        if (preConfig.find(req) == preConfig.end())
            throw std::runtime_error("Missing input field \"" + req + "\"");
    // Some inputs need to have a single input field
    static const std::vector<std::string> singles = {
        "dims", "domain", "pivot", "scale", "time", "iterations"
    };
    for (const std::string &single : singles)
        if (preConfig.at(single).size() != 1)
            throw std::runtime_error("Too many inputs given for \"" + single +
            "\"");
    // Number of equations, inits, boundaries and interval should be equal
    static const std::vector<std::string> eqs = {
        "equation", "init", "boundary", "interval"
    };
    size_t sz = preConfig.at(eqs.front()).size();
    for (const std::string &eq : eqs)
        if (preConfig.at(eq).size() != sz)
            throw std::runtime_error("Invalid number of \"" + eq + "\" entries "
            "given, should be " + std::to_string(sz));
}

double Parser::parseNum(const std::string &txt) const {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    const expr::ExprNode &root = parser.getTree();
    return root.eval();
}

void Parser::parseEquation(const std::string &txt) {
    expr::ExprNode root = parseExpr(txt);
    auto incorrect = []() -> void {
        throw std::runtime_error("Incorrect equation format");
    };
    if (root.type != expr::NODE_EQ)
        incorrect();
    if (root[0].type != expr::NODE_DERIV)
        incorrect();
    size_t semicol = root[0].content.find(';');
    if (semicol == std::string::npos)
        throw std::runtime_error("Internal error processing derivative");
    std::string deriv = root[0].content.substr(0, semicol);
    if (deriv != "t")
        incorrect();
    std::string var = root[0].content.substr(semicol + 1);
    if (std::find(system.vars.begin(), system.vars.end(), var) !=
    system.vars.end())
        throw std::runtime_error("Duplicate variable in equation");
    system.vars.push_back(var);
    system.vals.push_back(root[1]);
    system.init.emplace_back();
    system.boundary.emplace_back();
    system.bounds.push_back({-1.0, 1.0});
}

void Parser::parseAssignmentExpr(const std::string &txt,
std::vector<expr::ExprNode> &entry) {
    expr::ExprNode root = parseExpr(txt);
    auto incorrect = []() -> void {
        throw std::runtime_error("Incorrect equation format");
    };
    if (root.type != expr::NODE_EQ)
        incorrect();
    if (root[0].type != expr::NODE_SYMB)
        incorrect();
    std::string var = root[0].content;
    size_t loc = varIndex(var);
    if (loc == SIZE_MAX)
        std::runtime_error("Undefined variable \"" + var + "\"");
    entry[loc] = root[1];
}

void Parser::parseInterval(const std::string &txt) {
    expr::ExprNode root = parseExpr(txt);
    if (root.type != expr::NODE_EQ || root[0].type != expr::NODE_SYMB ||
    root[1].type != expr::NODE_LIST || root[1].size() != 2)
        throw std::runtime_error("Incorrect interval format");
    std::string var = root[0].content;
    size_t loc = varIndex(var);
    if (loc == SIZE_MAX)
        std::runtime_error("Undefined variable \"" + var + "\"");
    system.bounds[loc] = {root[1][0].eval(), root[1][1].eval()};
}

size_t Parser::varIndex(const std::string &var) const {
    for (size_t i = 0; i < system.vars.size(); i++)
        if (system.vars[i] == var)
            return i;
    return SIZE_MAX;
}

void Parser::parseDims(const std::string &txt) {
    expr::ExprNode root = parseExpr(txt);
    if (root.type != expr::NODE_LIST)
        throw std::runtime_error("Incorrect dims format");
    for (size_t i = 0; i < root.size(); i++) {
        if (root[i].type != expr::NODE_SYMB)
            throw std::runtime_error("Incorrect dims format");
        system.dims.push_back(root[i].content);
    }
}

expr::ExprNode Parser::parseExpr(const std::string &txt) const {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    return parser.getTree();
}

std::vector<double> Parser::parseNumList(const std::string &txt) const {
    expr::ExprNode root = parseExpr(txt);
    if (root.type != expr::NODE_LIST)
        throw std::runtime_error("Incorrect number list format");
    std::vector<double> out(root.size());
    for (size_t i = 0; i < root.size(); i++)
        out[i] = root[i].eval();
    return out;
}