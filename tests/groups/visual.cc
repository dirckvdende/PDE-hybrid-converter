/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

/**
 * This test outputs group divisions for each algorithm to a file to a given
 * folder where text files will be placed
 * 
 * This program should be executed from the root folder of this project!
 */

#include "dbg/dbg.h"
#include "pde/grid/groups/alg/alg.h"
#include "pde/grid/groups/alg/optimalrect.h"
#include "pde/grid/groups/alg/random.h"
#include "pde/grid/groups/alg/spread.h"
#include "pde/grid/groups/alg/squares.h"
#include "pde/grid/groups/depgrid.h"
#include "tests/groups.h"
#include <fstream>
#include <iostream>

std::string outputFolder;

/**
 * Object used produce visual ouput of algorithms
 */
class TestVisual final : public tests::groups::TestRunner {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid
     * @param range The dependency range of the grid
     * @param maxSize The maximum group size of the grid
     */
    TestVisual(std::vector<size_t> dims, std::vector<size_t> range, size_t
    maxSize) {
        this->dims = dims;
        this->range = range;
        this->maxSize = maxSize;
    }

    /**
     * Destructor
     */
    virtual ~TestVisual() { };

    /**
     * Run a specific test case, the grid has already been prepared
     * @param alg The division algorithm to use
     * @return A boolean indicating if the test was successful
     */
    virtual bool run(pde::grid::groups::alg::DivisionAlg *alg) {
        // Run algorithm and determine dependencies
        alg->run();
        grid->calc();
        // Output groups
        std::ofstream file(outputFolder + "/" + names[alg] + ".txt");
        if (!file.is_open())
            exit(1);
        file << ((pde::grid::groups::GroupGrid *)grid)->str();
        file.close();
        // Output border
        std::ofstream borderFile(outputFolder + "/" + names[alg] + "_border.txt");
        if (!borderFile.is_open())
            exit(1);
        borderFile << grid->str();
        borderFile.close();
        // Run visualization program
        system(("python3 tests/groups/graphic.py " + outputFolder + "/" +
        names[alg] + ".txt").c_str());
        system(("python3 tests/groups/graphic.py " + outputFolder + "/" +
        names[alg] + "_border.txt").c_str());
        return true;
    }

};

int main(int argc, char *argv[]) {
    dbg::DEBUG_ENABLED = true;
    outputFolder = "tmp";
    TestVisual test({100, 100}, {1, 1}, 168);
    test.setConfig(argc, argv);
    test.add<pde::grid::groups::alg::RandomAlg>("RandomAlg");
    test.add<pde::grid::groups::alg::SquaresAlg>("SquaresAlg");
    test.add<pde::grid::groups::alg::OptimalRectAlg>("OptimalRectAlg");
    test.add<pde::grid::groups::alg::SpreadAlg>("SpreadAlg");
    test.runAll();
    return 0;
}