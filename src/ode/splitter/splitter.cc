
#include "splitter.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
&system) const {
    std::unordered_map<std::string, size_t> varIndex;
    for (size_t i = 0; i < system.vars.size(); i++)
        varIndex.emplace(system.vars[i], i);
    std::vector<std::vector<size_t>> out(system.vars.size());
    for (size_t i = 0; i < system.vars.size(); i++) {
        std::unordered_set<std::string> names;
        system.vals[i].findAllSymbols(names);
        for (const std::string &name : names)
            if (varIndex.find(name) != varIndex.end())
                out[i].push_back(varIndex.at(name));
    }
    return out;
}

std::vector<std::vector<size_t>> Splitter::getStronglyConnected(const
std::vector<std::vector<size_t>> &deps) const {
    // Kosaraju's algorithm is used to determine the strongly connected
    // components of a graph. Source:
    // https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm
    std::vector<size_t> L;
    std::vector<std::vector<size_t>> rev(deps.size());
    std::vector<bool> done(deps.size(), false);
    std::vector<size_t> comp(deps.size(), SIZE_MAX);
    for (size_t i = 0; i < deps.size(); i++)
        for (size_t e : deps[i])
            rev[e].push_back(i);
    // Visits a node and then its neighbours
    std::function<void(size_t)> visit = [&](size_t i) -> void {
        if (done[i])
            return;
        done[i] = true;
        for (size_t e : deps[i])
            visit(e);
        L.push_back(i);
    };
    // Assign a node to a component
    std::function<void(size_t, size_t)> assign = [&](size_t i, size_t root) ->
    void {
        if (comp[i] != SIZE_MAX)
            return;
        comp[i] = root;
        for (size_t e : rev[i])
            assign(e, root);
    };
    for (size_t i = 0; i < deps.size(); i++)
        visit(i);
    std::reverse(L.begin(), L.end());
    for (size_t l : L)
        assign(l, l);
    std::vector<std::vector<size_t>> compList(deps.size());
    for (size_t i = 0; i < deps.size(); i++)
        compList[comp[i]].push_back(i);
    std::vector<std::vector<size_t>> compGraph(deps.size());
    for (size_t i = 0; i < deps.size(); i++)
        for (size_t e : deps[i])
            compGraph[comp[i]].push_back(comp[e]);
    std::vector<size_t> order = toposort(compGraph);
    std::reverse(order.begin(), order.end());
    std::vector<std::vector<size_t>> out;
    for (size_t i : order)
        if (!compList[i].empty())
            out.push_back(compList[i]);
    return out;
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
            cur.emit.push_back(newNames.back());
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

std::vector<size_t> Splitter::toposort(const std::vector<std::vector<size_t>>
&G) const {
    std::vector<std::vector<size_t>> rev(G.size());
    for (size_t i = 0; i < G.size(); i++)
        for (size_t e : G[i])
            rev[e].push_back(i);
    std::vector<size_t> out;
    std::vector<bool> done(G.size(), false);
    std::function<void(size_t)> dfs = [&](size_t i) -> void {
        done[i] = true;
        for (size_t e : rev[i])
            if (!done[e])
                dfs(e);
        out.push_back(i);
    };
    for (size_t i = 0; i < G.size(); i++)
        if (G[i].empty() && !done[i])
            dfs(i);
    return out;
}