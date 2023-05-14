
#include "compiler.h"
#include "grid/domain.h"
#include "parser/parser.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

Compiler::Compiler(int argc, char *argv[]) : args(argc, argv), domain(nullptr)
{ }

Compiler::~Compiler() { }

void Compiler::run() {
    debugLog("\nReading file contents...\n");
    readInputFile();
    debugLog("\nParsing input...\n");
    runParser();
    debugLog("\nDetermining domain...\n");
    findDomain();
    debugLog("");
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

void Compiler::runParser() {
    Parser parser(fileContents);
    parser.run();
    for (const std::pair<std::string, std::string> &field :
    parser.getFieldValues())
        debugLog("Read field: " + field.first + " -> " + field.second);
    debugLog("");
    debugLog(parser.getInputFields().str());
    inputFields = parser.getInputFields();
}

void Compiler::findDomain() {
    domain = new GridDomain(inputFields.scale, inputFields.domain,
    inputFields.dimensions);
    domain->findDomain(inputFields.pivot);
    // TODO: Implement border range detection
    domain->findBorder({1, 1});
    debugLog(domain->str());
}

void Compiler::debugLog(std::string txt) const {
    if (!args.getDebugMode())
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