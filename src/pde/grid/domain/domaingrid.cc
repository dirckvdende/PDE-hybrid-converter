
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