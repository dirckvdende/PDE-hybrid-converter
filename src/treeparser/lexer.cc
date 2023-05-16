
#include "lexer.h"
#include "settings.h"
#include <ostream>
#include <string>

using namespace treeparser;

Lexer::Lexer(std::string txt) : txt(txt), curIndex(0) {
    prepareConfigOptions();
}

Lexer::~Lexer() { }

void Lexer::clearText() {
    txt.clear();
}

void Lexer::append(std::string txt) {
    this->txt.append(txt);
}

void Lexer::updateSettings(const Settings &sett) {
    settings = sett;
}

void Lexer::run() {
    while (!atEnd()) {
        TokenType specialType;
        if (isWhitespaceChar(cur()))
            convertNameToken();
        if (isSpecialChar(cur(), specialType)) {
            convertNameToken();
            tokens.push_back({ specialType, "" });
        } else if (lastTokenWasText()) {
            tokens.back().content.push_back(cur());
        } else if (!isWhitespaceChar(cur())) {
            tokens.push_back({ TOK_TEXT, std::string(1, cur()) });
        }
        next();
    }
    convertNameToken();
}

const std::vector<Token> &Lexer::getTokens() const {
    return tokens;
}

char Lexer::cur() const {
    if (curIndex >= txt.size())
        return '\0';
    return txt[curIndex];
}

void Lexer::next() {
    curIndex++;
}

bool Lexer::atEnd() const {
    return curIndex >= txt.size();
}

void Lexer::convertNameToken() {
    if (tokens.empty())
        return;
    if (tokens.back().type != TOK_TEXT)
        return;
    if (configNames.find(tokens.back().content) == configNames.end())
        return;
    tokens.back().type = TOK_NAME;
}

void Lexer::prepareConfigOptions() {
    for (const ConfigOption &option : settings.configOptions)
        configNames.emplace(option.name, &option);
}

bool Lexer::lastTokenWasText() const {
    if (tokens.empty())
        return false;
    return tokens.back().type == TOK_TEXT;
}

bool Lexer::isSpecialChar(char c, TokenType &type) {
    switch (c) {
        case '{':
            type = TOK_LBRACE;
            return true;
        case '}':
            type = TOK_RBRACE;
            return true;
        case ';':
            type = TOK_SEMICOL;
            return true;
        default:
            break;
    }
    return false;
}

bool Lexer::isWhitespaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}