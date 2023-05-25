
#include "lexer.h"
#include <string>
#include <unordered_map>

using namespace expr;

Lexer::Lexer(std::string txt) : txt(txt) { }

Lexer::~Lexer() { }

void Lexer::run() {
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
    bool foundDot = false;
    tokens.push_back({TOK_NUM, ""});
    while (('0' <= cur() && cur() <= '9') || cur() == '.') {
        if (cur() == '.' && foundDot)
            return;
        if (cur() == '.')
            foundDot = true;
        tokens.back().content.push_back(cur());
        next();
    }
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