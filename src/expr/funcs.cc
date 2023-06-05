
#include "funcs.h"
#include <cmath>

using namespace expr;

const std::unordered_map<std::string, std::function<double(double)>> expr::funcs
= {
    {"sin", [](double x) -> double { return std::sin(x); }},
    {"cos", [](double x) -> double { return std::cos(x); }},
    {"tan", [](double x) -> double { return std::tan(x); }},
    {"exp", [](double x) -> double { return std::exp(x); }},
    {"abs", [](double x) -> double { return std::abs(x); }},
};