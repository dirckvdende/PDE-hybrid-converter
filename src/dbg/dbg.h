/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <string>

namespace dbg {

extern bool DEBUG_ENABLED;

/**
 * Output a debug message, if debugging is enabled
 * @param txt The message to print
 */
void log(const std::string &txt);

/**
 * Output an error and halt the program
 * @param txt The error message to print
 */
void error(const std::string &txt);

}