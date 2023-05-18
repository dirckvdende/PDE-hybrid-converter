
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

#include <iostream>
void Sim::runSystem(const ODESystem &system) {
    // Map from variables to index in the ODE system vars list
    std::unordered_map<std::string, size_t> names;
    for (size_t i = 0; i < system.vars.size(); i++) {
        if (names.find(system.vars[i]) != names.end())
            throw std::runtime_error("Duplicate variable name in system");
        names[system.vars[i]] = i;
    }
    // Determine variables that need to be emitted
    for (const auto &emit : system.emit)
        emitVals[emit.second] = {};
    // Keep track of values of variables
    std::unordered_map<std::string, double> vals;
    for (const auto &emit : emitVals)
        if (!emit.second.empty())
            vals[emit.first] = emit.second.front();
    // Determine initial values
    for (size_t i = 0; i < system.vals.size(); i++) {
        if (system.vals[i].type == expr::NODE_INTEG)
            vals[system.vars[i]] = system.vals[i][1].eval();
        else
            vals[system.vars[i]] = system.vals[i].evalDirect(vals);
    }
    // Run simulation
    size_t it = 0;
    for (double t = 0; t < system.time; t += stepSize, it++) {
        // Output emit vals
        for (const auto &emit : system.emit)
            emitVals[emit.second].push_back(vals[emit.first]);
        // Read emit vals
        for (const auto &emit : emitVals)
            vals[emit.first] = emit.second[it];
        for (size_t i = 0; i < system.vars.size(); i++) {
            const std::string &name = system.vars[i];
            if (system.vals[i].type == expr::NODE_INTEG)
                vals[name] += stepSize * system.vals[i][0].evalDirect(vals);
            else
                vals[name] = system.vals[i].evalDirect(vals);
            // Limit range
            vals[name] = std::max(vals[name], system.bounds[i].first);
            vals[name] = std::min(vals[name], system.bounds[i].second);
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