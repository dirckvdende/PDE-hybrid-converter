/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <string>

namespace expr {

/**
 * Type of an expression token
 */
enum TokenType {
    TOK_ERR,
    TOK_SYMB,
    TOK_DERIV,
    TOK_NUM,
    TOK_LBRACE, TOK_RBRACE,
    TOK_ADD, TOK_SUB, TOK_MUL, TOK_DIV,
    TOK_LT, TOK_GT, TOK_LTE, TOK_GTE, TOK_EQ,
    TOK_OR, TOK_AND,
    TOK_INTEG,
    TOK_COMMA, TOK_LBRACKET, TOK_RBRACKET,
    TOK_FUNC,
};

/**
 * Token in an expression, which has a type and "content", which can be literal
 * content or formatted information about the token
 */
struct Token {
    TokenType type;
    std::string content;
};

}