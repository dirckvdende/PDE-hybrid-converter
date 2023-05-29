
#include "generator.h"

using namespace pde::grid::generator;

ExprGenerator::ExprGenerator(Grid &grid, const PDESystem &system) : grid(grid),
system(system) { }

ExprGenerator::~ExprGenerator() { }
