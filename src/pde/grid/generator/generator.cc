/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "generator.h"

using namespace pde::grid::generator;

ExprGenerator::ExprGenerator(Grid &grid, const PDESystem &system) :
system(system), grid(grid) { }

ExprGenerator::~ExprGenerator() { }
