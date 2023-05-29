
#include "spec.h"
#include <string>

using namespace pde;

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
    out += "init        ->   [";
    for (size_t i = 0; i < init.size(); i++) {
        if (i != 0)
            out += ", ";
        out += init[i].str();
    }
    out += "]\n";
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
    out += "boundary    ->   [";
    for (size_t i = 0; i < boundary.size(); i++) {
        if (i != 0)
            out += ", ";
        out += boundary[i].str();
    }
    out += "]\n";
    out += "bounds      ->   [";
    for (size_t i = 0; i < bounds.size(); i++) {
        if (i != 0)
            out += ", ";
        out += "[" + std::to_string(bounds[i].first) + ", " +
        std::to_string(bounds[i].second) + "]";
    }
    out += "]";
    return out;
}