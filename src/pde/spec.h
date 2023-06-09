/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "expr/expr.h"
#include <string>
#include <vector>

namespace pde {

/**
 * Specification of a system of PDEs
 */
struct PDESystem {

    /**
     * Get a string representation of the stored input fields
     * @return The string representation
     */
    std::string str() const;

    // Names of the dimensions
    std::vector<std::string> dims;
    // Number of iterations to run
    size_t iterations;
    // Time to run the simulation for
    double time;
    // Initial values at t = 0
    std::vector<expr::ExprNode> init;
    // The size of each grid cell in the spatial dimensions
    double scale;
    // Pivot point, which should be in the domain
    std::vector<double> pivot;
    // Domain defition
    expr::ExprNode domain;
    // List of names of the PDE variables
    std::vector<std::string> vars;
    // System of PDEs
    std::vector<expr::ExprNode> vals;
    // Values at the boundary
    std::vector<expr::ExprNode> boundary;
    // Bounds on the values of the different variables
    std::vector<std::pair<double, double>> bounds;
    // Emit variables
    std::vector<std::pair<std::string, std::string>> emits;

};

}