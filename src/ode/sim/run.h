
#pragma once

namespace ode::sim {

/**
 * Check if help page needs to be shown, by checking for "-h" argument
 * @param argc Number of command line arguments
 * @param argv The command line arguments
 * @return A boolean indicating if the help argument was given
 */
bool checkHelp(int argc, char *argv[]);

/**
 * Show help page for ODE simulator
 */
void showHelp();

/**
 * Run the ode simulator given some command line arguments
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 * @return A status code, like returned by main()
 */
int run(int argc, char *argv[]);

}