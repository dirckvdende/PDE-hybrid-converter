
#pragma once

#include <string>

namespace dbg {

extern bool DEBUG_ENABLED;

/**
 * Output a debug message, if debugging is enabled
 * @param txt The message to print
 */
void log(const std::string &txt);

}