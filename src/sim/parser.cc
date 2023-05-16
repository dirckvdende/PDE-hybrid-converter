
#include "parser.h"
#include "treeparser/settings.h"
#include <stdexcept>

using namespace sim;

Parser::Parser() { }

Parser::~Parser() { }

void Parser::setText(const std::string &txt) {
    this->txt = txt;
    treeParser.setText(txt);
}

void Parser::run() {
    runTreeParser();
    // TODO: implement
}

void Parser::runTreeParser() {
    treeParser.setText(txt);
    static const treeparser::Settings settings = {
        {
            { "system", treeparser::OPT_TREE },
            { "var", treeparser::OPT_TEXT },
            { "interval", treeparser::OPT_TEXT },
            { "emit", treeparser::OPT_TEXT },
        }
    };
    treeParser.updateSettings(settings);
    treeParser.run();
    for (treeparser::ParseNode *node : treeParser.getRoot()->children) {
        if (node->name != "system")
            throw std::runtime_error("Missing \"system\" root node");
        preConfig.emplace_back();
        for (treeparser::ParseNode *child : node->children) {
            if (node->name == "system")
                throw std::runtime_error("Invalid \"system\" node location");
            preConfig.back().entries.push_back({child->name, child->text});
        }
    }
}