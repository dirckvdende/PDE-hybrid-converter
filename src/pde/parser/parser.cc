/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg/dbg.h"
#include "expr/expr.h"
#include "expr/parser.h"
#include "parser.h"
#include <algorithm>

using namespace pde;
using namespace pde::parser;

Parser::Parser() : validator(preParser.getSystem()),
transformer(preParser.getSystem()), systemGen(preParser.getSystem()) { }

Parser::~Parser() { }

void Parser::setText(const std::string &txt) {
    preParser.setText(txt);
}

void Parser::run() {
    dbg::log("Running preparser...");
    preParser.run();
    dbg::log(preParser.getSystem().str());
    dbg::log("Running input validator...");
    validator.run();
    dbg::log(preParser.getSystem().str());
    dbg::log("Running input transformer...");
    transformer.run();
    dbg::log(preParser.getSystem().str());
    dbg::log("Running PDE system generator...");
    systemGen.run();
}

const PDESystem &Parser::getSpecs() const {
    return systemGen.getSystem();
}