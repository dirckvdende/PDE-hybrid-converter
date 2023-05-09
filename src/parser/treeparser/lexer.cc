
#include "lexer.h"
#include "settings.h"
#include <ostream>
#include <string>

TreeLexer::TreeLexer(const std::string &txt) : txt(txt), curIndex(0) {
    prepareConfigOptions();
}

TreeLexer::~TreeLexer() { }

void TreeLexer::run() {
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

const std::vector<Token> &TreeLexer::getTokens() const {
    return tokens;
}

std::ostream &operator<<(std::ostream &os, const TreeLexer &lexer) {
    for (const Token &token : lexer.tokens) {
        switch (token.type) {
            case TOK_TEXT:
                os << "TEXT  ";
                break;
            case TOK_NAME:
                os << "NAME  ";
                break;
            case TOK_LBRACE:
                os << "{     ";
                break;
            case TOK_RBRACE:
                os << "}     ";
                break;
            case TOK_SEMICOL:
                os << ";     ";
                break;
        }
        os << token.content << '\n';
    }
    return os << std::flush;
}

char TreeLexer::cur() const {
    if (curIndex >= txt.size())
        return '\0';
    return txt[curIndex];
}

void TreeLexer::next() {
    curIndex++;
}

bool TreeLexer::atEnd() const {
    return curIndex >= txt.size();
}

void TreeLexer::convertNameToken() {
    if (tokens.empty())
        return;
    if (tokens.back().type != TOK_TEXT)
        return;
    if (configNames.find(tokens.back().content) == configNames.end())
        return;
    tokens.back().type = TOK_NAME;
}

void TreeLexer::prepareConfigOptions() {
    for (const TreeConfigOption &option : settings.configOptions)
        configNames.emplace(option.name, &option);
}

bool TreeLexer::lastTokenWasText() const {
    if (tokens.empty())
        return false;
    return tokens.back().type == TOK_TEXT;
}

bool TreeLexer::isSpecialChar(char c, TokenType &type) {
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

bool TreeLexer::isWhitespaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}