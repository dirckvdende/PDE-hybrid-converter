
#include "grid.h"

using namespace pde::grid;

Grid::Grid() { }

Grid::~Grid() { }

void Grid::setPivot(const std::vector<double> &val) {
    pivot = val;
}

void Grid::setScale(double val) {
    scale = val;
}