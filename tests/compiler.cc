
#include "tests/testing.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/**
 * Retrieve all of the test files contained in the test folder, recursive
 * @param testfolder The folder to locate all tests in, relative to the
 * "examples" folder
 * @return A vector of strings, containing all filenames
 */
std::vector<std::string> getTestFiles(const std::string &testfolder) {
    std::vector<std::string> out;
    for (fs::recursive_directory_iterator it("examples/" + testfolder), end;
    it != end; it++) {
        if (!fs::is_directory(it->path())) {
            std::string name = it->path().relative_path().string();
            if (it->path().extension() == ".pde")
                out.push_back(name);
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

int main() {
    std::vector<std::string> testfiles = getTestFiles("");
    tests::TestSuite suite;
    for (const std::string &file : testfiles)
        suite.append("build/main -d " + file);
    suite.run();
    return !suite.successful();
}