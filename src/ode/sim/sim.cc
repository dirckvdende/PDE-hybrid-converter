
#include "dbg/dbg.h"
#include "expr/expr.h"
#include "sim.h"
#include <chrono>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace ode::sim;

Sim::Sim() : specs(parser.getSpecs()), fileOutput(false), stepSize(1e-3) { }

Sim::~Sim() { }

void Sim::setText(const std::string &txt) {
    this->txt = txt;
    parser.setText(txt);
}

void Sim::setStepSize(double val) {
    stepSize = val;
}

void Sim::run() {
    resetStats();
    dbg::log("Parsing input...");
    auto tStart = std::chrono::high_resolution_clock::now();
    runParser();
    auto tRunStart = std::chrono::high_resolution_clock::now();
    stats.parseTime = std::chrono::duration_cast<std::chrono::microseconds>(
    tRunStart - tStart).count() / 1000000.0;
    for (const ODESystem &system : specs) {
        dbg::log("Running system...");
        runSystem(system);
    }
    auto tFileStart = std::chrono::high_resolution_clock::now();
    stats.systemTime = std::chrono::duration_cast<std::chrono::microseconds>(
    tFileStart - tRunStart).count() / 1000000.0;
    if (fileOutput) {
        dbg::log("Outputting to file...");
        outputEmit("tmp/ode.csv", 1000);
    }
    auto tEnd = std::chrono::high_resolution_clock::now();
    stats.fileTime = std::chrono::duration_cast<std::chrono::microseconds>(
    tEnd - tFileStart).count() / 1000000.0;
    logStats();
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
    auto tStart = std::chrono::high_resolution_clock::now();
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
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd -
    tStart);
    stats.iterations += it;
    stats.systemCount++;
    stats.emitCount += system.emit.size();
    stats.iterationTime += double(duration.count()) / 1000000.0;
}

void Sim::outputEmit(std::string filename, size_t resolution) const {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open output file");
    file << "__time__";
    for (const auto &emit : emitVals) {
        file << ',';
        file << emit.first;
    }
    // Gather data (includes time)
    std::vector<std::vector<double>> data;
    for (const auto &emit : emitVals) {
        size_t total = emit.second.size();
        for (size_t i = 0, it = 0; i < total; i += total / resolution, it++) {
            while (data.size() <= it)
                data.push_back({stepSize * i});
            data[it].push_back(emit.second[i]);
        }
    }
    // Output data as csv file
    for (const std::vector<double> &line : data) {
        file << '\n';
        bool first = true;
        for (const double &val : line) {
            if (!first)
                file << ',';
            first = false;
            file << val;
        }
    }
    file.close();
}

void Sim::resetStats() {
    stats.iterations = 0;
    stats.systemCount = 0;
    stats.emitCount = 0;
    stats.parseTime = 0.0;
    stats.iterationTime = 0.0;
    stats.systemTime = 0.0;
    stats.fileTime = 0.0;
}

void Sim::logStats() const {
    dbg::log("\nIterations:       " + std::to_string(stats.iterations));
    dbg::log("Systems:          " + std::to_string(stats.systemCount));
    dbg::log("Emits:            " + std::to_string(stats.emitCount));
    dbg::log("\nParse time:       " + std::to_string(stats.parseTime) + " s");
    dbg::log("Iteration time:   " + std::to_string(stats.iterationTime) + " s");
    dbg::log("System time:      " + std::to_string(stats.systemTime) + " s");
    dbg::log("File output time: " + std::to_string(stats.fileTime) + " s\n");
}