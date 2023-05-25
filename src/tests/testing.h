
#pragma once

#include <string>
#include <vector>

namespace tests {

/**
 * Text colors
 */
enum TextColor {
    COL_RED, COL_GREEN, COL_CYAN, COL_GREY,
};

/**
 * Output colored text
 * @param txt The text to color
 * @param color The color to give the text
 * @return Formatted text
 */
std::string colored(const std::string &txt, TextColor color);

/**
 * Object used to execute multiple commands and check if the returned status
 * code is 0
 */
class TestSuite {

public:

    /**
     * Constructor
     */
    TestSuite();

    /**
     * Destructor
     */
    ~TestSuite();

    /**
     * Add a command to the list of commands to run
     * @param cmd The command to append
     */
    void append(const std::string &cmd);

    /**
     * Run the test suite
     */
    void run();

    /**
     * After run() has been called, check if all tests have succeeded
     */
    bool successful() const;

private:

    /**
     * Test a specific command
     * @param cmd The command to run
     */
    void run(const std::string &cmd);

    // List of commands to run
    std::vector<std::string> commands;
    // Total runtime
    size_t runtime;
    // Succeeded and failed test counts
    size_t success, failed;

};

}