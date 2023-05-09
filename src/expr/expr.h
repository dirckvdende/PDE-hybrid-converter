
#pragma once

#include <string>
#include <vector>

/**
 * Expression node type
 */
enum ExprNodeType {
    NODE_ERR,
    NODE_SYMB,
    NODE_DERIV,
    NODE_ADD, NODE_SUB, NODE_MUL, NODE_DIV,
    NODE_LT, NODE_LTE, NODE_GT, NODE_GTE,
    NODE_EQ, NODE_NEQ,
};

/**
 * Node in an expression tree
 */
class ExprNode {

public:

    /**
     * Constructor
     * @param type The type of the node
     */
    ExprNode(const ExprNodeType type);

    /**
     * Destructor
     */
    ~ExprNode();

    /**
     * Copy constructor
     * @param other The expression to copy from
     */
    ExprNode(const ExprNode &other);

    /**
     * Check if two expressions are the same
     * @param other The other expression to compare
     * @return A boolean indicating if the expression is the same
     */
    bool operator==(const ExprNode &other) const;

    /**
     * Check if two expressions are unequal
     * @param other The other expression to compare
     * @return A boolean indicating if the expression is different
     */
    bool operator!=(const ExprNode &other) const;

    /**
     * Assignment copy
     * @param other The tree to copy from
     * @return A reference to this tree
     */
    ExprNode &operator=(const ExprNode &other);

    /**
     * Used to access a child of this node
     * @param index The index of the child to access
     * @return The child at the given index
     */
    ExprNode &operator[](size_t index);

    /**
     * Get the number of children of this expression node
     * @return The number of children
     */
    size_t size() const;

    /**
     * Convert this expression (back) to a string
     * @return A string representation of the expression
     */
    std::string str() const;

    // The node type
    ExprNodeType type;
    // Node children (pointers)
    std::vector<ExprNode *> children;
    // Node content (optional)
    std::string content;

private:

    /**
     * Convert a binary operator to a string
     * @return A string representation of the expression
     */
    std::string binaryStr() const;

};