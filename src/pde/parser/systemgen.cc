
#include "dbg/dbg.h"
#include "pde/spec.h"
#include "presystem.h"
#include "systemgen.h"

using namespace pde::parser;

SystemGen::SystemGen(const PreSystem &preSystem) : preSystem(preSystem) { }

SystemGen::~SystemGen() { }

void SystemGen::run() {
    system.dims = preSystem.dims;
    system.iterations = preSystem.iterations;
    system.time = preSystem.time;
    system.scale = preSystem.scale;
    system.pivot = preSystem.pivot;
    system.domain = preSystem.domain;
    system.emits = preSystem.emits;
    findVarNames();
    system.init.resize(system.vars.size());
    for (const expr::ExprNode &eq : preSystem.inits) {
        size_t index = varMap.at(eq[0].content);
        if (system.init[index].type != expr::NODE_ERR)
            dbg::error("Multiple init values given for variable");
        system.init[index] = eq[1];
    }
    if (preSystem.inits.size() != system.vars.size())
        dbg::error("Too few init values given");
    system.vals.resize(system.vars.size());
    for (const expr::ExprNode &eq : preSystem.equations) {
        size_t index = varMap[eq[0].deriv.var];
        if (system.vals[index].type != expr::NODE_ERR)
            dbg::error("Multiple equation values given for variable");
        system.vals[index] = eq[1];
    }
    if (preSystem.equations.size() != system.vars.size())
        dbg::error("Too few equations given");
    system.boundary.resize(system.vars.size());
    for (const expr::ExprNode &eq : preSystem.boundaries) {
        size_t index = varMap.at(eq[0].content);
        if (system.boundary[index].type != expr::NODE_ERR)
            dbg::error("Multiple boundary values given for variable");
        system.boundary[index] = eq[1];
    }
    if (preSystem.boundaries.size() != system.vars.size())
        dbg::error("Too few boundary values given");
    system.bounds = std::vector<std::pair<double, double>>(system.vars.size(),
    {0, 0});
    for (const expr::ExprNode &eq : preSystem.intervals) {
        size_t index = varMap.at(eq[0].content);
        if (system.bounds[index] != std::pair<double, double>{0, 0})
            dbg::error("Multiple interval values given for variable");
        system.bounds[index] = {eq[1][0].eval(), eq[1][1].eval()};
    }
    if (preSystem.intervals.size() != system.vars.size())
        dbg::error("Too few intervals given");
}

const pde::PDESystem &SystemGen::getSystem() const {
    return system;
}

void SystemGen::findVarNames() {
    for (const expr::ExprNode &eq : preSystem.equations) {
        std::string name = eq[0].deriv.var;
        varMap.emplace(name, system.vars.size());
        system.vars.push_back(name);
    }
}