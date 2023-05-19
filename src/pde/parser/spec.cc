
#include "spec.h"
#include <string>

using namespace pde::parser;

std::string PDESystem::str() const {
    std::string out;
    out += "dims        ->   [";
    for (size_t i = 0; i < dims.size(); i++) {
        if (i != 0)
            out += ", ";
        out += dims[i];
    }
    out += "]\n";
    out += "iterations  ->   " + std::to_string(iterations) + "\n";
    out += "time        ->   " + std::to_string(time) + "\n";
    out += "init        ->   " + init.str() + "\n";
    out += "boundary    ->   " + boundary.str() + "\n";
    out += "scale       ->   " + std::to_string(scale) + "\n";
    out += "pivot       ->   [";
    for (size_t i = 0; i < pivot.size(); i++) {
        if (i != 0)
            out += ", ";
        out += std::to_string(pivot[i]);
    }
    out += "]\n";
    out += "domain      ->   " + domain.str() + "\n";
    out += "vars        ->   [";
    for (size_t i = 0; i < vars.size(); i++) {
        if (i != 0)
            out += ", ";
        out += vars[i];
    }
    out += "]\n";
    out += "vals        ->   [";
    for (size_t i = 0; i < vals.size(); i++) {
        if (i != 0)
            out += ", ";
        out += vals[i].str();
    }
    out += "]\n";
    out += "bounds      ->   [";
    for (size_t i = 0; i < bounds.size(); i++) {
        if (i != 0)
            out += ", ";
        out += "[" + std::to_string(bounds[i].first) + ", " +
        std::to_string(bounds[i].second) + "]";
    }
    return out;
}