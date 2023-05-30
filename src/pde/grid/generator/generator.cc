
#include "generator.h"

using namespace pde::grid::generator;

ExprGenerator::ExprGenerator(Grid &grid, const PDESystem &system) :
system(system), grid(grid) { }

ExprGenerator::~ExprGenerator() { }
