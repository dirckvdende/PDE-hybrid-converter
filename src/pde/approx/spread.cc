
#include "spread.h"

using namespace pde;
using namespace pde::approx;

std::vector<size_t> pde::approx::calcSpread(const parser::PDESystem &system) {
    std::vector<size_t> out(system.dims.size(), 0);
    std::unordered_map<std::string, size_t> nameMap;
    for (size_t i = 0; i < system.dims.size(); i++)
        nameMap.emplace(system.dims[i], i);
    for (const expr::ExprNode &node : system.vals)
        calcSpread(&node, nameMap, out);
    return out;
}

void pde::approx::calcSpread(const expr::ExprNode *node, const
std::unordered_map<std::string, size_t> &nameMap, std::vector<size_t> &out) {
    if (node->type == expr::NODE_DERIV) {
        std::vector<size_t> cur(out.size(), 0);
        for (const std::string &dim : node->deriv.dims)
            cur[nameMap.at(dim)]++;
        for (size_t i = 0; i < out.size(); i++)
            out[i] = std::max(out[i], (cur[i] + 1) / 2);
        return;
    }
    for (const expr::ExprNode *child : node->children)
        calcSpread(child, nameMap, out);
}