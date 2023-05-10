
#pragma once

#include "expr.h"
#include "token.h"
#include <vector>

/**
 * Object used to parse a vector of tokens to a parse tree, which is also
 * represented as a vector
 */
class ExprParser {

public:

    /**
     * Constructor
     * @param tokens A reference to the input tokens
     */
    ExprParser(const std::vector<ExprToken> &tokens);

    /**
     * Destructor
     */
    ~ExprParser();

    /**
     * Run the equation parser
     */
    void run();

    /**
     * Get the output parse tree root node
     * @return A reference to the output parse tree as a vector
     */
    const ExprNode &getTree() const;

private:

    /**
     * Get the current token being read
     * @return The current token
     */
    ExprToken cur() const;

    /**
     * Move to the next token in the input vector
     */
    void next();

    /**
     * Check if the current position is past the end of the input vector
     * @return A boolean indicating pos is larger than the tokens vector size
     */
    bool atEnd() const;

    /**
     * Check if the type of the current token is of one of the given types
     * @param types The types to check for
     * @return A boolean indicating if the types match
     */
    bool accept(std::vector<ExprTokenType> types) const;

    /**
     * Require that the current token is of one of the given types
     * @param types The types to require
     */
    void expect(std::vector<ExprTokenType> types);

    /**
     * Read an (in)equality
     * @return A pointer to the generated expression parse node
     */
    ExprNode *readEquality();

    /**
     * Read a sum
     * @return A pointer to the generated expression parse node
     */
    ExprNode *readSum();

    /**
     * Read a product
     * @return A pointer to the generated expression parse node
     */
    ExprNode *readProduct();

    /**
     * Read a term in a product
     * @return A pointer to the generated expression parse node
     */
    ExprNode *readTerm();

    /**
     * Read a derivative
     * @return A pointer to the generated expression parse node
     */
    ExprNode *readDeriv();

    // Reference to the input tokens
    const std::vector<ExprToken> &tokens;
    // Output parse tree, last item is the root
    ExprNode tree;
    // Current position in the input stream
    size_t pos;

};