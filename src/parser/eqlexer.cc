
#include "eqlexer.h"
#include <stdexcept>
#include <string>
#include <unordered_map>

EquationLexer::EquationLexer(const std::string &txt) : txt(txt) { }

EquationLexer::~EquationLexer() { }

void EquationLexer::run() {
    pos = 0;
    while (!atEnd()) {
        if (isWhitespace(cur()))
            next();
        else if ('0' <= cur() && cur() <= '9')
            readNum();
        else if (isIdentChar(cur()))
            readIdent();
        else
            readSpecialChar();
    }
}

const std::vector<EquationToken> &EquationLexer::getTokens() const {
    return tokens;
}

char EquationLexer::cur() const {
    return pos < txt.size() ? txt[pos] : '\0';
}

void EquationLexer::next() {
    pos++;
}

bool EquationLexer::atEnd() const {
    return pos >= txt.size();
}

bool EquationLexer::isWhitespace(char c) {
    for (char w : "\n \t\r")
        if (c == w)
            return true;
    return false;
}

bool EquationLexer::isIdentChar(char c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') ||
    ('A' <= c && c <= 'Z') || c == '_';
}

void EquationLexer::readNum() {
    bool foundDot = false;
    tokens.push_back({EQTOK_NUM});
    while (('0' <= cur() && cur() <= '9') || cur() == '.') {
        if (cur() == '.' && foundDot)
            return;
        if (cur() == '.')
            foundDot = true;
        tokens.back().content.push_back(cur());
        next();
    }
}

void EquationLexer::readIdent() {
    tokens.push_back({cur() == 'd' ? EQTOK_DERIV : EQTOK_SYMB});
    while (isIdentChar(cur())) {
        tokens.back().content.push_back(cur());
        next();
    }
}

void EquationLexer::readSpecialChar() {
    static const std::unordered_map<std::string, EquationTokenType> typeMap = {
        { "(", EQTOK_LBRACE }, { ")", EQTOK_RBRACE }, { "+", EQTOK_ADD },
        { "-", EQTOK_SUB }, { "*", EQTOK_MUL }, { "/", EQTOK_DIV },
        { "<", EQTOK_LT }, { ">", EQTOK_GT }, { "<=", EQTOK_LTE },
        { ">=", EQTOK_GTE }, { "=", EQTOK_EQ },
    };
    std::string content;
    content.push_back(cur());
    next();
    while (typeMap.find(content + cur()) != typeMap.end())
        content.push_back(cur());
    tokens.push_back({typeMap.at(content)});
}