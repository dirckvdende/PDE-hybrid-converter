
#include "splitter.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace ode;
using namespace ode::splitter;

Splitter::Splitter() : uniqueCounter(0) { }

Splitter::~Splitter() { }

void Splitter::setSystems(const std::vector<ODESystem> &val) {
    systems = val;
}

void Splitter::run() {
    outputSystems.clear();
    for (const ODESystem &system : systems)
        splitSystem(system);
}

const std::vector<ODESystem> &Splitter::getOutputSystems() const {
    return outputSystems;
}

void Splitter::splitSystem(const ODESystem &system) {
    std::vector<std::vector<size_t>> deps = getDependencies(system);
    std::vector<std::vector<size_t>> comps = getStronglyConnected(deps);
    generateFromComponents(system, comps);
}

std::vector<std::vector<size_t>> Splitter::getDependencies(const ODESystem
&sytem) const {
    // TODO: implement
}

std::vector<std::vector<size_t>> Splitter::getStronglyConnected(const
std::vector<std::vector<size_t>> &deps) const {
    // TODO: implement
}

void Splitter::generateFromComponents(const ODESystem &system, const
std::vector<std::vector<size_t>> &comps) {
    std::unordered_map<std::string, std::string> tmpNames;
    std::unordered_map<std::string, std::vector<std::string>> emitMap;
    processEmits(system.emit, emitMap);
    for (const std::vector<size_t> &comp : comps) {
        std::vector<std::pair<std::string, std::string>> newNames;
        outputSystems.emplace_back();
        ODESystem &cur = outputSystems.back();
        for (const size_t &index : comp) {
            std::string name = system.vars[index];
            cur.vars.push_back(name);
            cur.vals.push_back(system.vals[index]);
            cur.vals.back().replaceSymbols(tmpNames);
            cur.bounds.push_back(system.bounds[index]);
            cur.time = system.time;
            if (emitMap.find(name) != emitMap.end())
                for (const std::string &emitName : emitMap[name])
                    cur.emit.push_back({name, emitName});
            newNames.push_back({name, getUniqueName()});
        }
        for (const std::pair<std::string, std::string> &name : newNames)
            tmpNames[name.first] = name.second;
    }
}

std::string Splitter::getUniqueName() {
    return "__v" + std::to_string(uniqueCounter++);
}

void Splitter::processEmits(const std::vector<std::pair<std::string,
std::string>> &emits, std::unordered_map<std::string, std::vector<std::string>>
&emitMap) const {
    for (const std::pair<std::string, std::string> &emit : emits) {
        if (emitMap.find(emit.first) == emitMap.end())
            emitMap.emplace(emit.first, std::vector<std::string>());
        emitMap[emit.first].push_back(emit.second);
    }
}