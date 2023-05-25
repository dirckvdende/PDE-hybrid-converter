
#include "testing.h"
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace tests;

#ifdef _WIN32
    #error "Windows is not supported!"
#else
std::string tests::colored(const std::string &txt, TextColor color) {
    switch (color) {
        case COL_RED:
            return "\033[0;31;1m" + txt + "\033[0m";
        case COL_GREEN:
            return "\033[0;32;1m" + txt + "\033[0m";
        case COL_CYAN:
            return "\033[0;36;1m" + txt + "\033[0m";
        case COL_GREY:
            return "\033[0;90m" + txt + "\033[0m";
        default:
            break;
    }
    return txt;
}
#endif

TestSuite::TestSuite() { }

TestSuite::~TestSuite() { }

void TestSuite::append(const std::string &cmd) {
    commands.push_back(cmd);
}

void TestSuite::run() {
    std::cout.precision(4);
    runtime = 0;
    success = failed = 0;
    for (const std::string &cmd : commands)
        run(cmd);
    std::cout << std::endl << colored("=======================================",
    failed == 0 ? COL_GREEN : COL_RED) << std::endl;
    if (failed == 0)
        std::cout << colored("ALL " + std::to_string(success) +
        " TEST(S) PASSED", COL_GREEN);
    else
        std::cout << colored(std::to_string(success) + " TEST(S) PASSED, " +
        std::to_string(failed) + " TEST(S) FAILED", COL_RED);
    std::cout << std::endl << std::endl;
}

bool TestSuite::successful() const {
    return failed == 0;
}

void TestSuite::run(const std::string &cmd) {
    auto startTime = std::chrono::high_resolution_clock::now();
    int exitStatus = system(cmd.c_str());
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>
    (endTime - startTime);
    runtime += duration.count();
    int exitCode = WEXITSTATUS(exitStatus);
    if (exitCode == 0 && duration.count() <= 100000000) {
        success++;
        std::cout << colored("[ SUCCESS ] ", COL_GREEN) << cmd << " (" <<
        (double(duration.count()) / 1000000.0) << " ms)" << std::endl;
    } else {
        failed++;
        std::cout << colored("[ FAILURE ] ", COL_RED) << cmd << std::endl;
        if (duration.count() > 100000000) {
            std::cout << colored("Test case took too long (" +
            std::to_string(duration.count() / 1000000) + " ms)", COL_RED) <<
            std::endl;
        }
    }
}