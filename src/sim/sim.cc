
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
    runParser();
    for (const ODESystem &system : specs)
        runSystem(system);
    if (fileOutput)
        outputEmit("tmp/ode.txt");
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
    std::vector<double> vals(system.vals.size());
    for (size_t i = 0; i < system.vals.size(); i++) {
        if (system.vals[i].type == expr::NODE_INTEG) {
            vals[i] = system.vals[i][1].eval();
        } else {
            expr::ExprNode node = system.vals[i];
            for (size_t j = 0; j < i; j++) {
                expr::ExprNode search(expr::NODE_SYMB, {}, system.vars[j]);
                expr::ExprNode repl(expr::NODE_NUM, {},
                std::to_string(vals[j]));
                node.replace(search, repl);
            }
        }
    }
    // Determine variables that need to be emitted
    std::vector<std::pair<size_t, std::string>> emits;
    for (const auto &emit : system.emit) {
        emits.push_back({names[emit.first], emit.second});
        emitVals[emit.second] = {};
    }
    // Run simulation
    for (double t = 0; t < system.time; t += stepSize) {
        for (const auto &emit : emits)
            emitVals[emit.second].push_back(vals[emit.first]);
        for (size_t i = 0; i < system.vals.size(); i++) {
            expr::ExprNode node;
            if (system.vals[i].type == expr::NODE_INTEG)
                node = system.vals[i][0];
            else
                node = system.vals[i];
            for (size_t j = 0; j < system.vals.size(); j++) {
                expr::ExprNode search(expr::NODE_SYMB, {}, system.vars[j]);
                expr::ExprNode repl(expr::NODE_NUM, {},
                std::to_string(vals[j]));
                node.replace(search, repl);
            }
            double val = node.eval();
            if (system.vals[i].type == expr::NODE_INTEG)
                vals[i] += stepSize * val;
            else
                vals[i] = val;
            // Limit range
            vals[i] = std::max(vals[i], system.bounds[i].first);
            vals[i] = std::min(vals[i], system.bounds[i].second);
        }
    }
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