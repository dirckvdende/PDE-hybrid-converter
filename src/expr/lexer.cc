
#include "lexer.h"
#include <stdexcept>
#include <string>
#include <unordered_map>

ExprLexer::ExprLexer(const std::string &txt) : txt(txt) { }

ExprLexer::~ExprLexer() { }

void ExprLexer::run() {
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

const std::vector<ExprToken> &ExprLexer::getTokens() const {
    return tokens;
}

char ExprLexer::cur() const {
    return pos < txt.size() ? txt[pos] : '\0';
}

void ExprLexer::next() {
    pos++;
}

bool ExprLexer::atEnd() const {
    return pos >= txt.size();
}

bool ExprLexer::isWhitespace(char c) {
    for (char w : "\n \t\r")
        if (c == w)
            return true;
    return false;
}

bool ExprLexer::isIdentChar(char c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') ||
    ('A' <= c && c <= 'Z') || c == '_';
}

void ExprLexer::readNum() {
    bool foundDot = false;
    tokens.push_back({EXPRTOK_NUM});
    while (('0' <= cur() && cur() <= '9') || cur() == '.') {
        if (cur() == '.' && foundDot)
            return;
        if (cur() == '.')
            foundDot = true;
        tokens.back().content.push_back(cur());
        next();
    }
}

void ExprLexer::readIdent() {
    tokens.push_back({cur() == 'd' ? EXPRTOK_DERIV : EXPRTOK_SYMB});
    while (isIdentChar(cur())) {
        tokens.back().content.push_back(cur());
        next();
    }
    if (tokens.back().content == "and") {
        tokens.back().content.clear();
        tokens.back().type = EXPRTOK_AND;
    }
    if (tokens.back().content == "or") {
        tokens.back().content.clear();
        tokens.back().type = EXPRTOK_OR;
    }
}

void ExprLexer::readSpecialChar() {
    static const std::unordered_map<std::string, ExprTokenType> typeMap = {
        { "(", EXPRTOK_LBRACE }, { ")", EXPRTOK_RBRACE }, { "+", EXPRTOK_ADD },
        { "-", EXPRTOK_SUB }, { "*", EXPRTOK_MUL }, { "/", EXPRTOK_DIV },
        { "<", EXPRTOK_LT }, { ">", EXPRTOK_GT }, { "<=", EXPRTOK_LTE },
        { ">=", EXPRTOK_GTE }, { "=", EXPRTOK_EQ },
    };
    std::string content;
    content.push_back(cur());
    next();
    while (typeMap.find(content + cur()) != typeMap.end())
        content.push_back(cur());
    tokens.push_back({typeMap.at(content)});
}