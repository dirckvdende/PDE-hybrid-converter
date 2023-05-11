
#pragma once

#include <string>
#include <vector>

/**
 * Class used to read and parse command line arguments
 */
class CmdArgs {

public:

    /**
     * Constructor
     * @param argc Number of command line arguments
     * @param argv Command line arguments
     */
    CmdArgs(int argc, char *argv[]);

    /**
     * Check if debug mode is enabled
     * @return A boolean indicating if debug mode is enabled
     */
    bool getDebugMode() const;

    /**
     * Get the input filename, shows an error and exits if filename is empty
     * @return The filename as a string
     */
    std::string getFilename() const;

private:

    /**
     * Parse command line arguments
     */
    void run();

    /**
     * Get the current command line argument, or an empty string if reading past
     * the end of the input
     * @return The current argument as a string
     */
    std::string cur() const;

    /**
     * Move to the next command line argument
     */
    void next();

    /**
     * Check if pos is past the end of the argument vector
     * @return A boolean indicating if pos is larger than the size of args
     */
    bool atEnd() const;

    /**
     * Read a single command line argment from the current position
     */
    void readArg();

    // Command line arguments
    std::vector<std::string> args;
    // Current reading index
    size_t pos;
    // Debug mode enabled
    bool debugMode;
    // Input filename
    std::string filename;

};