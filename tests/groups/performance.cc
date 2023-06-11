/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

/**
 * This program tests the time it takes for the algorithms to run
 */

#include "dbg/dbg.h"
#include "pde/grid/groups/alg/alg.h"
#include "pde/grid/groups/alg/random.h"
#include "pde/grid/groups/alg/squares.h"
#include "pde/grid/groups/alg/optimalrect.h"
#include "pde/grid/groups/alg/spread.h"
#include "pde/grid/groups/depgrid.h"
#include "tests/groups.h"
#include <chrono>
#include <iostream>

/**
 * Object used to test the performance of division algorithms
 */
class TestPerformance final : public tests::groups::TestRunner {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param range The dependency range of the grid
     * @param maxSize The maximum group size of the grid
     */
    TestPerformance(std::vector<size_t> dims, std::vector<size_t> range, size_t
    maxSize) {
        this->dims = dims;
        this->range = range;
        this->maxSize = maxSize;
    }

    /**
     * Destructor
     */
    virtual ~TestPerformance() { };

    /**
     * Run a specific test case, the grid has already been prepared
     * @param alg The division algorithm to use
     * @return A boolean indicating if the test was successful
     */
    virtual bool run(pde::grid::groups::alg::DivisionAlg *alg) {
        alg->run();
        grid->calc();
        size_t border = grid->dependCount();
        size_t interior = grid->size() - border;
        std::cout.precision(3);
        std::cout << std::fixed;
        std::cout << std::endl << names[alg] << ":" << std::endl;
        std::cout << "Number of border cells:    " << border << std::endl;
        std::cout << "Number of interior cells:  " << interior << std::endl;
        std::cout << "Ratio:                     " << double(interior) /
        double(border + interior) * 100.0 << " %" << std::endl << std::endl;
        return true;
    }

};

int main(int argc, char *argv[]) {
    dbg::DEBUG_ENABLED = true;
    TestPerformance test({100, 100}, {1, 1}, 168);
    test.setConfig(argc, argv);
    test.add<pde::grid::groups::alg::RandomAlg>("RandomAlg");
    test.add<pde::grid::groups::alg::SquaresAlg>("SquaresAlg");
    test.add<pde::grid::groups::alg::OptimalRectAlg>("OptimalRectAlg");
    test.add<pde::grid::groups::alg::SpreadAlg>("SpreadAlg");
    test.runAll();
    return 0;
}