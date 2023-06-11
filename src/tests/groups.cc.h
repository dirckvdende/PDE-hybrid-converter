/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "pde/grid/groups/alg/alg.h"
#include "testing.h"
#include "pde/grid/groups/depgrid.h"
#include <iostream>
#include <vector>

namespace tests::groups {

TestRunner::TestRunner() : grid(nullptr) { }

TestRunner::~TestRunner() {
    for (pde::grid::groups::alg::DivisionAlg *alg : algs)
        delete alg;
}

bool TestRunner::runAll() {
    bool res = true;
    std::cout << std::endl;
    for (pde::grid::groups::alg::DivisionAlg *alg : algs) {
        grid = new pde::grid::groups::DependGrid(dims, range, maxSize);
        alg->setGrid(*grid);
        if (run(alg)) {
            showSuccess(names[alg]);
        } else {
            showFail(names[alg]);
            res = false;
        }
        delete grid;
    }
    std::cout << std::endl;
    grid = nullptr;
    return res;
}

size_t TestRunner::size() const {
    return algs.size();
}

template<class Alg>
void TestRunner::add(std::string name) {
    Alg *alg = new Alg();
    algs.push_back(alg);
    names.emplace(alg, name);
}

void TestRunner::setConfig(int argc, char *argv[]) {
    if (argc >= 2)
        dims = splitNums(argv[1]);
    if (argc >= 3)
        range = splitNums(argv[2]);
    if (argc >= 4)
        maxSize = std::stoull(argv[3]);
    if (dims.size() != range.size())
        dbg::error("Dimensions and range of unequal length");
}

void TestRunner::showSuccess(std::string txt) {
    std::cout << "\033[1;32m[ SUCCESS ] \033[0m" << txt << std::endl;
}

void TestRunner::showFail(std::string txt) {
    std::cout << "\033[1;31m[ FAILURE ] \033[0m" << txt << std::endl;
}

std::vector<size_t> TestRunner::splitNums(std::string list) {
    size_t i = 0;
    std::vector<size_t> out;
    while (i != std::string::npos) {
        size_t start = i == 0 ? i : i + 1;
        size_t cur = list.find(',', start);
        out.push_back(std::stoull(list.substr(start, cur - start)));
        i = cur;
    }
    return out;
}

}