
#pragma once

namespace ode::sim {

/**
 * Run the ode simulator given some command line arguments
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 * @return A status code, like returned by main()
 */
int run(int argc, char *argv[]);

}