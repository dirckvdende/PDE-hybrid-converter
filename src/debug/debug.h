
#pragma once

#include <string>

extern bool DEBUG_ENABLED;

/**
 * Output a debug message, if debugging is enabled
 * @param txt The message to print
 */
void debugLog(const std::string &txt);