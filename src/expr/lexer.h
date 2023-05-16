
#pragma once

#include "token.h"
#include <vector>

namespace expr {

/**
 * Lexer for equations
 */
class Lexer {

public:

    /**
     * Constructor
     * @param txt A reference to the input text, should not be modified while
     * running the lexer
     */
    Lexer(const std::string &txt);

    /**
     * Destructor
     */
    ~Lexer();

    /**
     * Run the lexer, output tokens are stored
     */
    void run();

    /**
     * Get a reference to the vector of output tokens
     * @return A reference to the output token vector
     */
    const std::vector<Token> &getTokens() const;

private:

    /**
     * Get the character at the current position in the input text
     * @return The character as position pos, or '\0' if reading past the end of
     * the string
     */
    char cur() const;

    /**
     * Move to the next character in the input string
     */
    void next();

    /**
     * Check if the current position is past the end of the input string
     * @return A boolean indicating if pos is larger than the txt size
     */
    bool atEnd() const;

    /**
     * Check if a character is whitespace
     * @return A boolean indicating if a character is considered whitespace
     */
    static bool isWhitespace(char c);

    /**
     * Check if a character is valid for an identifier
     * @return A boolean indicating if a character is considered valid for an
     * identifier
     */
    static bool isIdentChar(char c);

    /**
     * Read a number token
     */
    void readNum();

    /**
     * Read an identifier or a derivative
     */
    void readIdent();

    /**
     * Read a special character, such as a brace or a plus sign
     */
    void readSpecialChar();

    // Input text
    const std::string &txt;
    // Output tokens
    std::vector<Token> tokens;
    // Current position in the input text
    size_t pos;

};

}