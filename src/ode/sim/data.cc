
#include "data.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace ode::sim;

SimData::SimData() { }

SimData::~SimData() { }

void SimData::add(const std::string &name) {
    size_t index;
    if (emitNames.find(name) != emitNames.end()) {
        index = emitNames.at(name);
        emitNames.erase(name);
        data[index].values.clear();
    } else {
        index = data.size();
        data.emplace_back();
    }
    names.emplace(name, index);
}

double SimData::get(size_t index, size_t step) const {
    return data[index].values[step];
}

size_t SimData::size(size_t index) const {
    return data[index].values.size();
}

size_t SimData::getIndex(const std::string &name) const {
    if (names.find(name) != names.end())
        return names.at(name);
    return emitNames.at(name);
}

void SimData::append(size_t index, double val) {
    data[index].values.push_back(val);
}

void SimData::reset() {
    for (DataEntry &entry : data)
        if (entry.emitRefs == 0)
            entry.values.clear();
    names.clear();
}

void SimData::addEmit(const std::string &inp, const std::string &out) {
    size_t index = names.at(inp);
    if (emitNames.find(out) != emitNames.end())
        data[emitNames.at(out)].emitRefs--;
    emitNames.emplace(out, index);
    data[index].emitRefs++;
}

std::vector<std::pair<std::string, size_t>> SimData::emitList() const {
    std::vector<std::pair<std::string, size_t>> out;
    for (const std::pair<std::string, size_t> &emit : emitNames)
        if (!emit.first.empty() && emit.first.front() != '_')
            out.push_back(emit);
    return out;
}