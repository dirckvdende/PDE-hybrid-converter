
#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace expr {

// Special function list, mapping names to functions to call
extern const std::unordered_map<std::string, std::function<double(double)>>
funcs;

}