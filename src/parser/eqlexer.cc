
#include "eqlexer.h"
#include <string>

EquationLexer::EquationLexer(const std::string &txt) : txt(txt) { }

EquationLexer::~EquationLexer() { }

void EquationLexer::run() {

}

const std::vector<EquationToken> &EquationLexer::getTokens() const {
    return tokens;
}