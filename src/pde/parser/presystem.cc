
#include "presystem.h"

using namespace pde::parser;

std::string PreSystem::str() const {
    std::string out;
    out += "dims        ->   [";
    for (size_t i = 0; i < dims.size(); i++) {
        if (i != 0)
            out += ", ";
        out += dims[i];
    }
    out += "]\n";
    out += "domain      ->   " + domain.str() + "\n";
    out += "pivot       ->   [";
    for (size_t i = 0; i < pivot.size(); i++) {
        if (i != 0)
            out += ", ";
        out += std::to_string(pivot[i]);
    }
    out += "]\n";
    out += "scale       ->   " + std::to_string(scale) + "\n";
    out += "time        ->   " + std::to_string(time) + "\n";
    out += "iterations  ->   " + std::to_string(iterations) + "\n";
    out += "equations   ->   [";
    for (size_t i = 0; i < equations.size(); i++) {
        if (i != 0)
            out += ", ";
        out += equations[i].str();
    }
    out += "]\n";
    out += "inits       ->   [";
    for (size_t i = 0; i < inits.size(); i++) {
        if (i != 0)
            out += ", ";
        out += inits[i].str();
    }
    out += "]\n";
    out += "boundaries  ->   [";
    for (size_t i = 0; i < boundaries.size(); i++) {
        if (i != 0)
            out += ", ";
        out += boundaries[i].str();
    }
    out += "]\n";
    out += "intervals   ->   [";
    for (size_t i = 0; i < intervals.size(); i++) {
        if (i != 0)
            out += ", ";
        out += intervals[i].str();
    }
    out += "]\n";
    out += "emits       ->   [";
    for (size_t i = 0; i < emits.size(); i++) {
        if (i != 0)
            out += ", ";
        out += emits[i].first + " as " + emits[i].second;
    }
    out += "]\n";
    return out;
}