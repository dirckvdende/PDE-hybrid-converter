
#include "dbg/dbg.h"
#include "funcs.h"
#include "lexer.h"
#include <string>
#include <unordered_map>
#include <iostream>

using namespace expr;

Lexer::Lexer(std::string txt) : txt(txt) { }

Lexer::~Lexer() { }

void Lexer::run() {
    pos = 0;
    while (!atEnd()) {
        if (isWhitespace(cur()))
            next();
        else if (('0' <= cur() && cur() <= '9') || cur() == '.')
            readNum();
        else if (isIdentChar(cur()))
            readIdent();
        else
            readSpecialChar();
    }
}

void Lexer::clearText() {
    txt = "";
}

void Lexer::append(const std::string &txt) {
    this->txt.append(txt);
}

const std::vector<Token> &Lexer::getTokens() const {
    return tokens;
}

char Lexer::cur() const {
    return pos < txt.size() ? txt[pos] : '\0';
}

void Lexer::next() {
    pos++;
}

bool Lexer::atEnd() const {
    return pos >= txt.size();
}

bool Lexer::isWhitespace(char c) {
    for (char w : "\n \t\r")
        if (c == w)
            return true;
    return false;
}

bool Lexer::isIdentChar(char c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') ||
    ('A' <= c && c <= 'Z') || c == '_';
}

void Lexer::readNum() {
    tokens.emplace_back();
    tokens.back().type = TOK_NUM;
    tokens.back().content.append(readSubNum());
    if (cur() != 'e' && cur() != 'E')
        return;
    tokens.back().content.push_back('e');
    next();
    std::string exp = readSubNum();
    if (exp == "-")
        dbg::log("Invalid number notation with only \"-\" in exponent");
    tokens.back().content.append(exp);
}

std::string Lexer::readSubNum() {
    std::string out;
    if (cur() == '-') {
        out.push_back(cur());
        next();
    }
    bool foundDot = false;
    while (('0' <= cur() && cur() <= '9') || cur() == '.') {
        if (cur() == '.' && foundDot)
            return out;
        if (cur() == '.')
            foundDot = true;
        out.push_back(cur());
        next();
    }
    return out;
}

void Lexer::readIdent() {
    tokens.push_back({cur() == 'd' ? TOK_DERIV : TOK_SYMB, ""});
    while (isIdentChar(cur())) {
        tokens.back().content.push_back(cur());
        next();
    }
    if (tokens.back().content == "and") {
        tokens.back().content.clear();
        tokens.back().type = TOK_AND;
    }
    if (tokens.back().content == "or") {
        tokens.back().content.clear();
        tokens.back().type = TOK_OR;
    }
    if (tokens.back().content == "integ") {
        tokens.back().content.clear();
        tokens.back().type = TOK_INTEG;
    }
    if (funcs.find(tokens.back().content) != funcs.end())
        tokens.back().type = TOK_FUNC;
}

void Lexer::readSpecialChar() {
    static const std::unordered_map<std::string, TokenType> typeMap = {
        { "(", TOK_LBRACE }, { ")", TOK_RBRACE }, { "+", TOK_ADD },
        { "-", TOK_SUB }, { "*", TOK_MUL }, { "/", TOK_DIV },
        { "<", TOK_LT }, { ">", TOK_GT }, { "<=", TOK_LTE },
        { ">=", TOK_GTE }, { "=", TOK_EQ }, { ",", TOK_COMMA },
        { "[", TOK_LBRACKET }, { "]", TOK_RBRACKET },
    };
    std::string content;
    content.push_back(cur());
    next();
    while (typeMap.find(content + cur()) != typeMap.end())
        content.push_back(cur());
    tokens.push_back({typeMap.at(content), ""});
}