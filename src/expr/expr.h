
#pragma once

#include <string>
#include <vector>

/**
 * Expression node type
 */
enum ExprNodeType {
    NODE_ERR,
    NODE_SYMB,
    NODE_NUM,
    NODE_DERIV,
    NODE_ADD, NODE_SUB, NODE_MUL, NODE_DIV,
    NODE_LT, NODE_LTE, NODE_GT, NODE_GTE,
    NODE_EQ, NODE_NEQ,
    NODE_AND, NODE_OR,
    NODE_MINUS,
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
    ExprNode(ExprNodeType type = NODE_ERR);

    /**
     * Constructor
     * @param type The type of the node
     * @param children Pointers to the children of the parse node
     * @param content The content of the parse node
     */
    ExprNode(ExprNodeType type, const std::vector<ExprNode *> &children,
    std::string content = "");

    /**
     * Copy constructor
     * @param other The expression to copy from
     */
    ExprNode(const ExprNode &other);

    /**
     * Destructor
     */
    ~ExprNode();

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
     * @note If the index is larger than the children vector size, the vector
     * will be extended with error nodes
     */
    ExprNode &operator[](size_t index);

    /**
     * Used to access a child of this node as a constant reference
     * @param index The index of the child to access
     * @return A constant reference to the child at the given index
     * @warning Will throw an error if index if too high
     */
    const ExprNode &operator[](size_t index) const;

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

    /**
     * Find all occurrences of a specific subtree. Complexity of this operation
     * is O(mn), where n and m are the sizes of this tree and the tree to search
     * for
     * @param other The tree to search for
     * @param occ A vector that all of the occurrences will be appended to
     */
    void find(const ExprNode &other, std::vector<ExprNode *> &occ);

    /**
     * Substitute all occurrences of a specific subtree with another. Complexity
     * of this operation is O(mn), where n and m are the sizes of this tree and
     * the tree to search for
     * @param search The tree to replace
     * @param repl The tree to replace with
     */
    void replace(const ExprNode &search, const ExprNode &repl);

    /**
     * Evaluate an expression. If there are still derivatives or symbols left,
     * an error is thrown
     * @return The evaluated value as a double. Other types are implicitly
     * coverted to boolean
     */
    double eval() const;

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