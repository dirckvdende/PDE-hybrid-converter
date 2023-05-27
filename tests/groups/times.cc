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

using namespace std::chrono;

/**
 * Object used to test the execution times of division algorithms
 */
class TestTimes final : public tests::groups::TestRunner {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param range The dependency range of the grid
     * @param maxSize The maximum group size of the grid
     */
    TestTimes(std::vector<size_t> dims, std::vector<size_t> range, size_t
    maxSize) {
        this->dims = dims;
        this->range = range;
        this->maxSize = maxSize;
    }

    /**
     * Destructor
     */
    virtual ~TestTimes() { };

    /**
     * Run a specific test case, the grid has already been prepared
     * @param alg The division algorithm to use
     * @return A boolean indicating if the test was successful
     */
    virtual bool run(pde::grid::groups::alg::DivisionAlg *alg) {
        auto t0 = high_resolution_clock::now();
        alg->run();
        auto t1 = high_resolution_clock::now();
        grid->calc();
        auto t2 = high_resolution_clock::now();
        auto d1 = duration_cast<microseconds>(t1 - t0);
        auto d2 = duration_cast<microseconds>(t2 - t1);
        std::cout.precision(8);
        std::cout << std::fixed;
        std::cout << std::endl << names[alg] << ":" << std::endl;
        std::cout << "Algorithm run: " << (double(d1.count()) / 1000000.0)
        << std::endl;
        std::cout << "Result check:  " << (double(d2.count()) / 1000000.0)
        << std::endl;
        std::cout << "Total:         " << (double(d1.count() + d2.count()) /
        1000000.0) << std::endl << std::endl;
        return true;
    }

};

int main(int argc, char *argv[]) {
    TestTimes test({100, 100}, {1, 1}, 121);
    test.setConfig(argc, argv);
    test.add<pde::grid::groups::alg::RandomAlg>("random");
    test.add<pde::grid::groups::alg::SquaresAlg>("squares");
    test.add<pde::grid::groups::alg::OptimalRectAlg>("rect");
    test.add<pde::grid::groups::alg::SpreadAlg>("spread");
    test.add<pde::grid::groups::alg::RectSpreadAlg>("rect-spread");
    test.runAll();
    return 0;
}