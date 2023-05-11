
#include "cmdargs.h"
#include <iostream>

CmdArgs::CmdArgs(int argc, char *argv[]) : debugMode(false) {
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);
    run();
}

bool CmdArgs::getDebugMode() const {
    return debugMode;
}

std::string CmdArgs::getFilename() const {
    if (filename.empty()) {
        std::cerr << "Usage: " << args[0] << " [options] <filename>" <<
        std::endl;
        exit(1);
    }
    return filename;
}

void CmdArgs::run() {
    pos = 1;
    while (!atEnd())
        readArg();
}

std::string CmdArgs::cur() const {
    if (pos >= args.size())
        return "";
    return args[pos];
}

void CmdArgs::next() {
    pos++;
}

bool CmdArgs::atEnd() const {
    return pos >= args.size();
}

void CmdArgs::readArg() {
    if (cur() == "-d" || cur() == "--debug") {
        debugMode = true;
        next();
        return;
    }
    if (cur().front() == '-') {
        std::cerr << "Invalid option \"" << cur() << "\"" << std::endl;
        exit(1);
    }
    filename = cur();
}