
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "sim.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace sim;

Sim::Sim() : specs(parser.getSpecs()), fileOutput(false), stepSize(1e-2) { }

Sim::~Sim() { }

void Sim::setText(const std::string &txt) {
    this->txt = txt;
    parser.setText(txt);
}

void Sim::run() {
    dbg::log("Parsing input...");
    runParser();
    for (const ODESystem &system : specs) {
        dbg::log("Running system...");
        runSystem(system);
    }
    if (fileOutput) {
        dbg::log("Outputting to file...");
        outputEmit("tmp/ode.txt", 1000);
    }
}

void Sim::setFileOutput(bool val) {
    fileOutput = val;
}

void Sim::setStepSize(bool val) {
    stepSize = val;
}

void Sim::runParser() {
    parser.run();
}

void Sim::runSystem(const ODESystem &system) {
    // Make a copy of the equations
    std::vector<expr::ExprNode> equations;
    for (size_t i = 0; i < system.vals.size(); i++)
        equations.push_back(system.vals[i]);
    // Keep track of names of variables and emit history
    std::vector<std::string> names;
    std::unordered_map<std::string, size_t> nameMap;
    std::vector<std::vector<double>> history;
    for (const auto &emit : emitVals) {
        if (nameMap.find(emit.first) == nameMap.end()) {
            nameMap.emplace(emit.first, names.size());
            names.push_back(emit.first);
            history.push_back(emit.second);
        }
    }
    size_t varStart = names.size();
    for (size_t i = 0; i < system.vars.size(); i++) {
        if (nameMap.find(system.vars[i]) != nameMap.end())
            std::runtime_error("Duplicate variable in system");
        nameMap.emplace(system.vars[i], names.size());
        names.push_back(system.vars[i]);
    }
    for (expr::ExprNode &eq : equations)
        eq.replaceSymbols(nameMap);
    // Determine initial values
    std::vector<double> vals;
    for (size_t i = 0; i < varStart; i++)
        vals.push_back(history[i].front());
    for (size_t i = varStart; i < names.size(); i++) {
        if (equations[i - varStart].type == expr::NODE_INTEG)
            vals.push_back(equations[i - varStart][1].eval());
        else
            vals.push_back(equations[i - varStart].evalDirect(vals));
        history.push_back({vals[i]});
    }
    // Run simulation
    size_t it = 0;
    for (double t = 0; t < system.time; t += stepSize, it++) {
        // Read history values
        for (size_t i = 0; i < varStart; i++)
            vals[i] = history[i][it];
        for (size_t i = varStart; i < names.size(); i++) {
            if (equations[i - varStart].type == expr::NODE_INTEG)
                vals[i] += stepSize * equations[i - varStart][0].evalDirect(
                vals);
            else
                vals[i] = equations[i - varStart].evalDirect(vals);
            // Limit range
            vals[i] = std::max(vals[i], system.bounds[i - varStart].first);
            vals[i] = std::min(vals[i], system.bounds[i - varStart].second);
            history[i].push_back(vals[i]);
        }
    }
    // Output emit values
    for (const auto &emit : system.emit)
        emitVals[emit.second] = history[nameMap[emit.first]];
    dbg::log("Total number of iterations: " + std::to_string(it));
}

void Sim::outputEmit(std::string filename, size_t resolution) const {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open output file");
    for (const auto &emit : emitVals) {
        file << emit.first;
        size_t total = emit.second.size();
        for (size_t i = 0; i < total; i += total / resolution)
            file << ' ' << emit.second[i];
        file << '\n';
    }
    file.close();
}