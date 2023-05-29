
#include "grid.h"
#include "pde/spec.h"

using namespace pde::grid;

Grid::Grid() { }

Grid::~Grid() { }

std::vector<double> Grid::toRealLoc(const GridCell *cell) {
    std::vector<size_t> loc = toLoc(cell);
    std::vector<double> out(loc.size());
    for (size_t i = 0; i < loc.size(); i++)
        out[i] = loc[i] * system.scale + pivot[i];
    return out;
}

std::vector<double> Grid::toRealLoc(const GridCell &cell) {
    return toRealLoc(&cell);
}