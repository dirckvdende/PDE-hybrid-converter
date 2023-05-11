
#include "compiler.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

Compiler::Compiler(int argc, char *argv[]) : args(argc, argv) { }

Compiler::~Compiler() { }

void Compiler::run() {
    debugLog("Reading file contents...");
    readInputFile();
}

void Compiler::readInputFile() {
    std::ifstream file(args.getFilename());
    if (!file.is_open())
        throw std::runtime_error("Could not open given file");
    std::stringstream stream;
    stream << file.rdbuf();
    fileContents = stream.str();
    file.close();
}

void Compiler::debugLog(std::string txt) const {
    if (!args.getDebugMode())
        return;
    time_t t = std::time(nullptr);
    tm *tm = std::localtime(&t);
    std::cout << "\033[90m[" << std::put_time(tm, "%H:%M:%S") << "]\033[0m ";
    std::cout << txt << std::endl;
}