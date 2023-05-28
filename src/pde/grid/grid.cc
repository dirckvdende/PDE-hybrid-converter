
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

std::vector<double> Grid::toRealLoc(const GridCell *cell) {
    std::vector<size_t> loc = toLoc(cell);
    std::vector<double> out(loc.size());
    for (size_t i = 0; i < loc.size(); i++)
        out[i] = loc[i] * scale + pivot[i];
    return out;
}

std::vector<double> Grid::toRealLoc(const GridCell &cell) {
    return toRealLoc(&cell);
}