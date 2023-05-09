
#pragma once

#include <string>

/**
 * Type of an equation token
 */
enum EquationTokenType {
    EQTOK_ERR,
    EQTOK_SYMB,
    EQTOK_DERIV,
    EQTOK_NUM,
    EQTOK_LBRACE, EQTOK_RBRACE,
    EQTOK_ADD, EQTOK_SUB, EQTOK_MUL, EQTOK_DIV,
    EQTOK_LT, EQTOK_GT, EQTOK_LTE, EQTOK_GTE, EQTOK_EQ,
};

/**
 * Token in an equation, which has a type and "content", which can be literal
 * content or formatted information about the token
 */
struct EquationToken {
    EquationTokenType type;
    std::string content;
};