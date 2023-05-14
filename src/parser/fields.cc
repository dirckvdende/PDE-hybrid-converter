
#include "fields.h"
#include <string>

std::string InputFields::str() const {
    std::string out;
    out += "iterations -> " + std::to_string(iterations) + "\n";
    out += "time       -> " + std::to_string(time) + "\n";
    out += "init       -> " + init.str() + "\n";
    out += "boundary   -> " + boundary.str() + "\n";
    out += "scale      -> " + std::to_string(scale) + "\n";
    out += "pivot      -> [";
    for (size_t i = 0; i < pivot.size(); i++) {
        if (i != 0)
            out += ", ";
        out += std::to_string(pivot[i]);
    }
    out += "]\n";
    out += "domain     -> " + domain.str() + "\n";
    out += "func       -> " + func.str() + "\n";
    out += "equation   -> " + equation.str() + "\n";
    out += "dimensions -> [";
    for (size_t i = 0; i < dimensions.size(); i++) {
        if (i != 0)
            out += ", ";
        out += "\"" + dimensions[i] + "\"";
    }
    out += "]\n";
    return out;
}