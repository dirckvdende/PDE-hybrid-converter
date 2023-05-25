
#include "splitter.h"

using namespace ode;
using namespace ode::splitter;

Splitter::Splitter() { }

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
    // TODO: implement
}