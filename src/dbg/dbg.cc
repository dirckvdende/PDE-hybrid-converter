
#include "dbg.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

bool DEBUG_ENABLED = false;

void debugLog(const std::string &txt) {
    if (!DEBUG_ENABLED)
        return;
    auto showTime = []() -> void {
        time_t t = std::time(nullptr);
        tm *tm = std::localtime(&t);
        std::cout << "\033[90m[" << std::put_time(tm, "%H:%M:%S") <<
        "]\033[0m ";
    };
    showTime();
    for (const char &c : txt) {
        std::cout << c;
        if (c == '\n')
            showTime();
    }
    std::cout << std::endl;
}