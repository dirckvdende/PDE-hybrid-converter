/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

namespace ode::splitter {

/**
 * Run the ode splitter given some command line arguments
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 * @return A status code, like returned by main()
 */
int run(int argc, char *argv[]);

}