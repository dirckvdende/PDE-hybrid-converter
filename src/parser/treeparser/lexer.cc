
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
        TreeTokenType specialType;
        if (isWhitespaceChar(cur()))
            convertNameToken();
        if (isSpecialChar(cur(), specialType)) {
            convertNameToken();
            tokens.push_back({ specialType, "" });
        } else if (lastTokenWasText()) {
            tokens.back().content.push_back(cur());
        } else if (!isWhitespaceChar(cur())) {
            tokens.push_back({ TREETOK_TEXT, std::string(1, cur()) });
        }
        next();
    }
    convertNameToken();
}

const std::vector<TreeToken> &TreeLexer::getTokens() const {
    return tokens;
}

std::ostream &operator<<(std::ostream &os, const TreeLexer &lexer) {
    for (const TreeToken &token : lexer.tokens) {
        switch (token.type) {
            case TREETOK_TEXT:
                os << "TEXT  ";
                break;
            case TREETOK_NAME:
                os << "NAME  ";
                break;
            case TREETOK_LBRACE:
                os << "{     ";
                break;
            case TREETOK_RBRACE:
                os << "}     ";
                break;
            case TREETOK_SEMICOL:
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
    if (tokens.back().type != TREETOK_TEXT)
        return;
    if (configNames.find(tokens.back().content) == configNames.end())
        return;
    tokens.back().type = TREETOK_NAME;
}

void TreeLexer::prepareConfigOptions() {
    for (const TreeConfigOption &option : settings.configOptions)
        configNames.emplace(option.name, &option);
}

bool TreeLexer::lastTokenWasText() const {
    if (tokens.empty())
        return false;
    return tokens.back().type == TREETOK_TEXT;
}

bool TreeLexer::isSpecialChar(char c, TreeTokenType &type) {
    switch (c) {
        case '{':
            type = TREETOK_LBRACE;
            return true;
        case '}':
            type = TREETOK_RBRACE;
            return true;
        case ';':
            type = TREETOK_SEMICOL;
            return true;
        default:
            break;
    }
    return false;
}

bool TreeLexer::isWhitespaceChar(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}