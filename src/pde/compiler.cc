
#include "compiler.h"
#include "dbg/dbg.h"
#include "grid/domain/domain.h"
#include "parser/parser.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace pde;

Compiler::Compiler(int argc, char *argv[]) : args(argc, argv), domain(nullptr)
{ }

Compiler::~Compiler() { }

void Compiler::run() {
    dbg::log("\nReading file contents...\n");
    readInputFile();
    dbg::log("\nParsing input...\n");
    runParser();
    dbg::log("\nDetermining domain...\n");
    findDomain();
    dbg::log("");
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
    parser::Parser parser;
    parser.setText(fileContents);
    parser.run();
}

void Compiler::findDomain() {
    domain = new grid::domain::GridDomain(inputFields.scale, inputFields.domain,
    inputFields.dims);
    domain->findDomain(inputFields.pivot);
    // TODO: Implement border range detection
    domain->findBorder({1, 1});
    dbg::log(domain->str());
}