
#include "dbg/dbg.h"
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
    debugLog("Parsing input...");
    runParser();
    for (const ODESystem &system : specs) {
        debugLog("Running system...");
        runSystem(system);
    }
    if (fileOutput) {
        debugLog("Outputting to file...");
        outputEmit("tmp/ode.txt");
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
    // Map from variables to index in the ODE system vars list
    std::unordered_map<std::string, size_t> names;
    for (size_t i = 0; i < system.vars.size(); i++) {
        if (names.find(system.vars[i]) != names.end())
            throw std::runtime_error("Duplicate variable name in system");
        names[system.vars[i]] = i;
    }
    // Determine initial values
    std::vector<double> vals;
    for (size_t i = 0; i < system.vals.size(); i++) {
        if (system.vals[i].type == expr::NODE_INTEG) {
            vals.push_back(system.vals[i][1].eval());
        } else {
            expr::ExprNode node = system.vals[i];
            replaceSymbols(node, system, vals, 0);
            vals.push_back(node.eval());
        }
    }
    // Determine variables that need to be emitted
    std::vector<std::pair<size_t, std::string>> emits;
    for (const auto &emit : system.emit) {
        emits.push_back({names[emit.first], emit.second});
        emitVals[emit.second] = {};
    }
    // Run simulation
    size_t it = 0;
    for (double t = 0; t < system.time; t += stepSize, it++) {
        for (const auto &emit : emits)
            emitVals[emit.second].push_back(vals[emit.first]);
        for (size_t i = 0; i < system.vals.size(); i++) {
            expr::ExprNode node;
            if (system.vals[i].type == expr::NODE_INTEG)
                node = system.vals[i][0];
            else
                node = system.vals[i];
            replaceSymbols(node, system, vals, it);
            if (system.vals[i].type == expr::NODE_INTEG)
                vals[i] += stepSize * node.eval();
            else
                vals[i] = node.eval();
            // Limit range
            vals[i] = std::max(vals[i], system.bounds[i].first);
            vals[i] = std::min(vals[i], system.bounds[i].second);
        }
    }
    debugLog("Total number of iterations: " + std::to_string(it));
}

void Sim::outputEmit(std::string filename) const {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open output file");
    for (const auto &emit : emitVals) {
        file << emit.first;
        for (const double &v : emit.second)
            file << ' ' << v;
        file << '\n';
    }
    file.close();
}

void Sim::replaceSymbols(expr::ExprNode &node, const ODESystem &system,
const std::vector<double> &vals, size_t it) const {
    for (size_t i = 0; i < vals.size(); i++)
        node.replaceSymbol(system.vars[i], vals[i]);
    for (const auto &emit : emitVals)
        node.replaceSymbol(emit.first, emit.second[it]);
}