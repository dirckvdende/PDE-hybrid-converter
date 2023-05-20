
#include "domaingrid.h"
#include <vector>

using namespace pde::grid::domain;

DomainGrid::DomainGrid(std::vector<size_t> dims) :
hypergrid::HyperGrid<DomainInfo>(dims), scale(1) {
    for (size_t i = 0; i < dims.size(); i++)
        pivot.push_back(0.0);
}

DomainGrid::~DomainGrid() { }

void DomainGrid::setPivot(const std::vector<double> &val) {
    pivot = val;
}

void DomainGrid::setScale(double val) {
    scale = val;
}

std::vector<double> DomainGrid::toRealCoords(const std::vector<size_t> &coords)
const {
    std::vector<double> out(coords.size());
    for (size_t i = 0; i < coords.size(); i++)
        out[i] = pivot[i] + scale * coords[i];
    return out;
}

std::vector<size_t> DomainGrid::toGridCoords(const std::vector<double> &coords)
const {
    std::vector<size_t> out(coords.size());
    for (size_t i = 0; i < coords.size(); i++)
        out[i] = size_t((coords[i] - pivot[i]) / scale + 0.5);
    return out;
}