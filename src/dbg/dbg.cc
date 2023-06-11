/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#include "dbg.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

using namespace dbg;

bool dbg::DEBUG_ENABLED = false;

void dbg::log(const std::string &txt) {
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

void dbg::error(const std::string &txt) {
    std::cerr << std::endl << "\033[91m" << "error: ";
    for (const char &c : txt) {
        std::cerr << c;
        if (c == '\n')
            std::cerr << "    ";
    }
    std::cerr << std::endl << std::endl;
    exit(1);
}