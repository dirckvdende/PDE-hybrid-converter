
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
    size_t index = 1;
    for (const ODESystem &system : specs) {
        dbg::log("Running system " + std::to_string(index) + " out of " +
        std::to_string(specs.size()) + "...");
        runSystem(system);
        index++;
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

void Sim::runSystem(ODESystem system) {
    auto tStart = std::chrono::high_resolution_clock::now();
    // Add variables of current system to dataframe
    for (const std::string &var : system.vars)
        dataframe.add(var);
    // System variable index to dataframe index
    std::vector<size_t> dataIndex(system.vars.size());
    for (size_t i = 0; i < system.vars.size(); i++)
        dataIndex[i] = dataframe.getIndex(system.vars[i]);
    // Add emits of the current system
    for (const std::pair<std::string, std::string> emit : system.emit)
        dataframe.addEmit(emit.first, emit.second);
    replaceVars(system);
    generateInitCond(system, dataIndex);
    size_t step = 1;
    for (double t = stepSize; t < system.time; t += stepSize, step++)
        runStep(system, step, dataIndex);
    dataframe.reset();
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd -
    tStart);
    stats.iterations += step;
    stats.systemCount++;
    stats.emitCount += system.emit.size();
    stats.iterationTime += double(duration.count()) / 1000000.0;
}

void Sim::replaceVars(ODESystem &system) {
    for (expr::ExprNode &val : system.vals)
        val.walk([&](expr::ExprNode &node) -> void {
            if (node.type == expr::NODE_SYMB) {
                node.type = expr::NODE_VAR_MARKER;
                node.index = dataframe.getIndex(node.content);
                node.content.clear();
            }
        });
}

void Sim::generateInitCond(const ODESystem &system, const std::vector<size_t>
&dataIndex) {
    for (size_t i = 0; i < system.vars.size(); i++) {
        if (system.vals[i].type == expr::NODE_INTEG) {
            dataframe.append(dataIndex[i], system.vals[i][1].eval());
        } else {
            double val = system.vals[i].evalDirect([&](const expr::ExprNode
            &node) -> double {
                if (node.type != expr::NODE_VAR_MARKER ||
                dataframe.size(node.index) == 0)
                    dbg::error("Could not determine intial "
                    "value of expression \"" + node.str() + "\"");
                return dataframe.get(node.index, 0);
            });
            dataframe.append(dataIndex[i], val);
        }
    }
}

void Sim::runStep(const ODESystem &system, size_t step, const
std::vector<size_t> &dataIndex) {
    for (size_t i = 0; i < system.vars.size(); i++) {
        double val;
        if (system.vals[i].type == expr::NODE_INTEG) {
            double change = system.vals[i][0].evalDirect([&](const
            expr::ExprNode &node) -> double {
                if (node.type != expr::NODE_VAR_MARKER ||
                step - 1 >= dataframe.size(node.index))
                    dbg::error("Could not determine value of expression \"" +
                    node.str() + "\"");
                return dataframe.get(node.index, step - 1);
            });
            if (step != dataframe.size(dataIndex[i]))
                dbg::error("Could not determine value of expression \"" +
                system.vals[i].str() + "\"");
            val = dataframe.get(dataIndex[i], step - 1) + change * stepSize;
        } else {
            val = system.vals[i].evalDirect([&](const expr::ExprNode
            &node) -> double {
                if (node.type != expr::NODE_VAR_MARKER ||
                step >= dataframe.size(node.index))
                    dbg::error("Could not determine value of expression \"" +
                    node.str() + "\"");
                return dataframe.get(node.index, step);
            });
        }
        const std::pair<double, double> bound = system.bounds[i];
        dataframe.append(dataIndex[i], std::min(std::max(val, bound.first),
        bound.second));
    }
}

void Sim::outputEmit(std::string filename, size_t resolution) {
    std::ofstream file(filename);
    if (!file.is_open())
        dbg::error("Could not open output file \"" + filename + "\"");
    file << "__time__";
    std::vector<std::pair<std::string, size_t>> emits = dataframe.emitList();
    for (const std::pair<std::string, size_t> &emit : emits) {
        file << ',';
        file << emit.first;
    }
    // Gather data (includes time)
    std::vector<std::vector<double>> data;
    for (const std::pair<std::string, size_t> &emit : emits) {
        size_t total = dataframe.size(emit.second);
        for (size_t i = 0, it = 0; i < total; i += std::max(total / resolution,
        1UL), it++) {
            while (data.size() <= it)
                data.push_back({stepSize * i});
            data[it].push_back(dataframe.get(emit.second, i));
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