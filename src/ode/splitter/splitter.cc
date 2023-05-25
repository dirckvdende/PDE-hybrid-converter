
#include "splitter.h"

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
    // TODO: implement
}

std::string Splitter::getUniqueName() {
    return "__v" + std::to_string(uniqueCounter++);
}