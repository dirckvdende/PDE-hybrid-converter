
#pragma once

#include <string>

/**
 * Type of an expression token
 */
enum ExprTokenType {
    EXPRTOK_ERR,
    EXPRTOK_SYMB,
    EXPRTOK_DERIV,
    EXPRTOK_NUM,
    EXPRTOK_LBRACE, EXPRTOK_RBRACE,
    EXPRTOK_ADD, EXPRTOK_SUB, EXPRTOK_MUL, EXPRTOK_DIV,
    EXPRTOK_LT, EXPRTOK_GT, EXPRTOK_LTE, EXPRTOK_GTE, EXPRTOK_EQ,
    EXPRTOK_OR, EXPRTOK_AND,
};

/**
 * Token in an expression, which has a type and "content", which can be literal
 * content or formatted information about the token
 */
struct ExprToken {
    ExprTokenType type;
    std::string content;
};