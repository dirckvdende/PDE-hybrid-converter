
#pragma once

#include "token.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Equation parse node type
 */
enum EquationNodeType {
    NODE_ERR,
    NODE_NUM, NODE_SYMB,
    NODE_DERIV,
    NODE_ADD, NODE_SUB, NODE_MUL, NODE_DIV,
    NODE_LT, NODE_GT, NODE_LTE, NODE_GTE, NODE_EQ,
};

/**
 * Equation parse node, which can have contents and children (indices)
 */
struct EquationNode {
    EquationNodeType type;
    std::string content;
    std::vector<size_t> children;
};

/**
 * Equation parse tree, which is a derived class from a vector of equation
 * nodes, for extra functionality
 */
class EquationTree : public std::vector<EquationNode> {

public:

    /**
     * Evaluate the (sub)tree given certain values
     * @param vals The values of the symbols in the tree
     * @param node The index of the node to take as root (default root node)
     * @return The evaluated value of the parse tree root as a double, for
     * binary values this will be 0.0 or 1.0
     */
    double eval(const std::unordered_map<std::string, double> &vals, size_t root
    = SIZE_MAX) const;

private:

};

/**
 * Object used to parse a vector of tokens to a parse tree, which is also
 * represented as a vector
 */
class EquationParser {

public:

    /**
     * Constructor
     * @param tokens A reference to the input tokens
     */
    EquationParser(const std::vector<EquationToken> &tokens);

    /**
     * Destructor
     */
    ~EquationParser();

    /**
     * Run the equation parser
     */
    void run();

    /**
     * Get the output parse tree as a vector, where the last item is the root
     * @return A reference to the output parse tree as a vector
     */
    const EquationTree &getTree() const;

private:

    /**
     * Get the current token being read
     * @return The current token
     */
    EquationToken cur() const;

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
    bool accept(std::vector<EquationTokenType> types) const;

    /**
     * Require that the current token is of one of the given types
     * @param types The types to require
     */
    void expect(std::vector<EquationTokenType> types);

    /**
     * Read an (in)equality
     * @return The index of the parse node
     */
    size_t readEquality();

    /**
     * Read a sum
     * @return The index of the parse node
     */
    size_t readSum();

    /**
     * Read a product
     * @return The index of the parse node
     */
    size_t readProduct();

    /**
     * Read a term in a product
     * @return The index of the parse node
     */
    size_t readTerm();

    /**
     * Read a derivative
     * @return The index of the parse node
     */
    size_t readDeriv();

    // Reference to the input tokens
    const std::vector<EquationToken> &tokens;
    // Output parse tree, last item is the root
    EquationTree tree;
    // Current position in the input stream
    size_t pos;

};