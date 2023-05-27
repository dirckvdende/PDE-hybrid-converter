/**
 * This program tests the time it takes for the algorithms to run
 */

#include "pde/grid/groups/alg/alg.h"
#include "pde/grid/groups/alg/random.h"
#include "pde/grid/groups/alg/squares.h"
#include "pde/grid/groups/alg/optimalrect.h"
#include "pde/grid/groups/alg/spread.h"
#include "pde/grid/groups/alg/rectspread.h"
#include "pde/grid/groups/depgrid.h"
#include "tests/groups.h"
#include <chrono>
#include <iostream>

/**
 * A configuration for a specific test case when bulk testing
 */
struct BulkConfig {
    std::vector<size_t> dims, range;
    size_t maxSize;
};

/**
 * Object used to test the performance of division algorithms
 */
class TestBulk final : public tests::groups::TestRunner {

public:

    /**
     * Constructor
     */
    TestBulk() { prepareConfig(); }

    /**
     * Destructor
     */
    virtual ~TestBulk() { }

    /**
     * Run all test cases and output results in CSV format
     * @return A boolean indicating if tests were successful
     */
    virtual bool runAll() {
        std::cout.precision(5);
        std::cout << std::fixed;
        std::cout << "maxSize";
        for (pde::grid::groups::alg::DivisionAlg *alg : algs)
            std::cout << ',' << names[alg];
        std::cout << std::endl;
        for (const auto &config : configs) {
            std::cout << config.maxSize;
            for (pde::grid::groups::alg::DivisionAlg *alg : algs) {
                double result = 1.0 - runConfig(alg, config);
                std::cout << ',' << result;
            }
            std::cout << std::endl;
        }
        return true;
    }

    /**
     * Run a specific test case, for a specific configuration of dimensions,
     * range and maximum group size
     * @param alg The division algorithm to use
     * @param config The configuration to run
     * @return The proportion of interior cells
     */
    double runConfig(pde::grid::groups::alg::DivisionAlg *alg,
    const BulkConfig &config) {
        dims = config.dims;
        range = config.range;
        maxSize = config.maxSize;
        grid = new pde::grid::groups::DependGrid(dims, range, maxSize);
        alg->setGrid(*grid);
        std::cout.setstate(std::ios_base::failbit);
        alg->run();
        grid->calc();
        size_t border = grid->dependCount();
        size_t interior = grid->size() - border;
        std::cout.clear();
        delete grid;
        return double(interior) / double(border + interior);
    }
    
    /**
     * Disabled, included to prevent compitler error
     */
    virtual bool run(pde::grid::groups::alg::DivisionAlg *alg) {
        (void)alg;
        return true;
    }

    /**
     * Set the current configurations, this functions should be changed when
     * wanting to run different tests
     */
    void prepareConfig() {
        for (size_t i = 4; i <= 300; i += 4)
            configs.push_back({{256, 256}, {2, 1}, i});
    }

    // Configuration for which grid sizes, max group sizes, and ranges to use
    std::vector<BulkConfig> configs;

};

int main() {
    TestBulk test;
    test.add<pde::grid::groups::alg::RandomAlg>("RandomAlg");
    test.add<pde::grid::groups::alg::SquaresAlg>("SquaresAlg");
    test.add<pde::grid::groups::alg::OptimalRectAlg>("OptimalRectAlg");
    test.add<pde::grid::groups::alg::SpreadAlg>("SpreadAlg");
    test.add<pde::grid::groups::alg::RectSpreadAlg>("RectSpreadAlg");
    test.runAll();
    return 0;
}