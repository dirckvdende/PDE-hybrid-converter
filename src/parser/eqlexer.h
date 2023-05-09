
#pragma once

#include "token.h"
#include <vector>

/**
 * Lexer for equations
 */
class EquationLexer {

public:

    /**
     * Constructor
     * @param txt A reference to the input text, should not be modified while
     * running the lexer
     */
    EquationLexer(const std::string &txt);

    /**
     * Destructor
     */
    ~EquationLexer();

    /**
     * Run the lexer, output tokens are stored
     */
    void run();

    /**
     * Get a reference to the vector of output tokens
     * @return A reference to the output token vector
     */
    const std::vector<EquationToken> &getTokens() const;

private:

    // Input text
    const std::string &txt;
    // Output tokens
    std::vector<EquationToken> tokens;

};