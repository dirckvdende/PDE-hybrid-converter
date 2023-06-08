
#include "dbg/dbg.h"
#include "expr/parser.h"
#include "preparser.h"

using namespace pde::parser;

PreParser::PreParser() { }

PreParser::~PreParser() { }

void PreParser::setText(const std::string &val) {
    txt = val;
    treeParser.setText(txt);
}

void PreParser::run() {
    runTreeParser();
    runExprParser();
}

const PreSystem &PreParser::getSystem() const {
    return preSystem;
}

void PreParser::runTreeParser() {
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
            { "emit", treeparser::OPT_TEXT },
        }
    };
    treeParser.updateSettings(settings);
    try {
        treeParser.run();
    } catch (const std::runtime_error &err) {
        dbg::error("Invalid configuration structure or entries");
    }
    if (treeParser.getRoot()->children.size() != 1)
        dbg::error("Cannot handle multiple PDE systems");
    treeparser::ParseNode *root = treeParser.getRoot()->children.front();
    if (root->name != "pde")
        dbg::error("Missing \"pde\" root node in configuration");
    for (treeparser::ParseNode *child : root->children) {
        if (child->name == "pde")
            dbg::error("Invalid \"pde\" node location in configuration");
        if (preConfig.find(child->name) == preConfig.end())
            preConfig.emplace(child->name, std::vector<std::string>{});
        preConfig[child->name].push_back(child->text);
    }
}

void PreParser::runExprParser() {
    checkRequiredFields();
    parseDims(preConfig.at("dims").front());
    preSystem.domain = parseExpr(preConfig.at("domain").front());
    preSystem.pivot = parseNumList(preConfig.at("pivot").front());
    preSystem.scale = std::stod(preConfig["scale"].front());
    for (const std::string &entry : preConfig.at("equation"))
        preSystem.equations.push_back(parseExpr(entry));
    for (const std::string &entry : preConfig.at("init"))
        preSystem.inits.push_back(parseExpr(entry));
    for (const std::string &entry : preConfig.at("boundary"))
        preSystem.boundaries.push_back(parseExpr(entry));
    for (const std::string &entry : preConfig.at("interval"))
        preSystem.intervals.push_back(parseExpr(entry));
    if (preConfig.find("emit") != preConfig.end())
        for (const std::string &entry : preConfig.at("emit"))
            parseEmit(entry);
    preSystem.time = std::stod(preConfig["time"].front());
    preSystem.iterations = std::stoull(preConfig["iterations"].front());
}

void PreParser::checkRequiredFields() const {
    // Check all required fields
    static const std::vector<std::string> required = {
        "dims", "domain", "pivot", "scale", "equation", "init", "boundary",
        "interval", "time", "iterations"
    };
    for (const std::string &req : required)
        if (preConfig.find(req) == preConfig.end())
            dbg::error("Missing input field \"" + req + "\"");
    // Some inputs need to have a single input field
    static const std::vector<std::string> singles = {
        "dims", "domain", "pivot", "scale", "time", "iterations"
    };
    for (const std::string &single : singles)
        if (preConfig.at(single).size() != 1)
            dbg::error("Too many inputs given for \"" + single + "\"");
}

void PreParser::parseDims(const std::string &txt) {
    expr::ExprNode root = parseExpr(txt);
    if (root.type != expr::NODE_LIST)
        dbg::error("Incorrect dimsions format, must be list");
    for (size_t i = 0; i < root.size(); i++) {
        if (root[i].type != expr::NODE_SYMB)
            dbg::error("Incorrect dimsions format, invalid entry");
        preSystem.dims.push_back(root[i].content);
    }
}

void PreParser::parseEmit(const std::string &txt) {
    size_t first = txt.find(' ');
    size_t second = txt.find(' ', first + 1);
    if (first == txt.npos || second == txt.npos)
        dbg::error("Could not parse emit statement");
    std::string inp = txt.substr(0, first);
    std::string as = txt.substr(first + 1, second);
    std::string out = txt.substr(second + 1);
    if (as != "as")
        dbg::error("Could not parse emit statement");
    preSystem.emits.push_back({inp, out});
}

expr::ExprNode PreParser::parseExpr(const std::string &txt) const {
    expr::Parser parser;
    parser.setText(txt);
    parser.run();
    return parser.getTree();
}

std::vector<double> PreParser::parseNumList(const std::string &txt) const {
    expr::ExprNode root = parseExpr(txt);
    if (root.type != expr::NODE_LIST)
        dbg::error("Incorrect number list format, should be a list");
    std::vector<double> out(root.size());
    for (size_t i = 0; i < root.size(); i++)
        out[i] = root[i].eval();
    return out;
}