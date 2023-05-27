
#pragma once

#include "pde/grid/groups/alg/alg.h"
#include <unordered_map>
#include <vector>

namespace tests::groups {

/**
 * Component to run a suite of tests
 * Is configurable to enable/disable output and change some configurations
 */
class TestRunner {

public:

    /**
     * Constructor
     */
    TestRunner();

    /**
     * Destructor
     */
    virtual ~TestRunner();

    /**
     * Run all test cases
     * @return A boolean indicating if the tests were successful
     */
    virtual bool runAll();

    /**
     * Run a specific test case
     * @param alg The division algorithm to use
     * @return A boolean indicating if the test was successful
     */
    virtual bool run(pde::grid::groups::alg::DivisionAlg *alg) = 0;

    /**
     * Get the number of test cases
     * @return The number of test cases
     */
    size_t size() const;

    /**
     * Add a test case to the test runner
     * @tparam Alg The type of algorithm to test, must be derived from
     * DivisionAlg class
     * @param name The name to give the division algorithm test
     */
    template<class Alg>
    void add(std::string name);

    /**
     * Set the configuration of this test based on command line arguments
     * First argument should be dimensions, second the range, and third the
     * maximum group size
     * @param argc The number of command line arguments
     * @param argv The command line arguments
     */
    void setConfig(int argc, char *argv[]);

protected:

    /**
     * Display success text to the console
     * @param txt The text to show
     */
    void showSuccess(std::string txt);

    /**
     * Display failure text to the console
     * @param txt The text to show
     */
    void showFail(std::string txt);

    /**
     * Split a list of numbers separated by ','
     * @param list The list of numbers as a string separated by ','
     * @return The list of numbers as a vector
     */
    static std::vector<size_t> splitNums(std::string list);

    // All algorithms to run
    std::vector<pde::grid::groups::alg::DivisionAlg *> algs;
    // Names of the algorithms
    std::unordered_map<pde::grid::groups::alg::DivisionAlg *, std::string>
    names;
    // The dimensions of the grid
    std::vector<size_t> dims;
    // The dependency range of the grid
    std::vector<size_t> range;
    // The maximum group size of the grid
    size_t maxSize;

    // The grid that is currently being used, will be nullptr outside execution
    // of run()
    pde::grid::groups::DependGrid *grid;

};

}

#include "groups.cc.h"