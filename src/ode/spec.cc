
#include "spec.h"
#include <string>

using namespace ode;

std::string ODESystem::str() const {
    std::string out = "system {\n";
    for (size_t i = 0; i < vars.size(); i++)
        out.append("    var " + vars[i] + " = " + vals[i].str() + ";\n");
    for (const std::pair<std::string, std::string> &e : emit)
        out.append("    emit " + e.first + " as " + e.second + ";\n");
    for (size_t i = 0; i < vars.size(); i++)
        out.append("    interval " + vars[i] + " = [" +
        std::to_string(bounds[i].first) + ", " +
        std::to_string(bounds[i].second) + "];\n");
    out.append("    time " + std::to_string(time) + ";\n");
    out.append("}");
    return out;
}