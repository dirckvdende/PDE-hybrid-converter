
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace expr {

/**
 * Expression node type
 */
enum NodeType {
    NODE_ERR,
    NODE_SYMB,
    NODE_NUM,
    NODE_DERIV,
    NODE_ADD, NODE_SUB, NODE_MUL, NODE_DIV,
    NODE_LT, NODE_LTE, NODE_GT, NODE_GTE,
    NODE_EQ, NODE_NEQ,
    NODE_AND, NODE_OR,
    NODE_MINUS,
    NODE_INTEG, NODE_LIST,
    // Dimension index indicator, uses index
    NODE_DIM,
    // Variable marker: replacement of a variable with an index
    NODE_VAR_MARKER,
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
    ExprNode(NodeType type = NODE_ERR);

    /**
     * Constructor
     * @param type The type of the node
     * @param children Pointers to the children of the parse node
     * @param content The content of the parse node
     */
    ExprNode(NodeType type, const std::vector<ExprNode *> &children,
    std::string content = "");

    /**
     * Constructor
     * @param type The type of the node
     * @param children Pointers to the children of the parse node
     * @param number The numeric content of the parse node
     */
    ExprNode(NodeType type, const std::vector<ExprNode *> &children,
    double number);

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
     * Replace symbols with other symbols
     * @param symbols A map from symbol names to new symbol names
     */
    void replaceSymbols(const std::unordered_map<std::string, std::string>
    &symbols);

    /**
     * Get all symbols names in this expression tree
     * @param symbols A set where the symbol names should be put
     */
    void findAllSymbols(std::unordered_set<std::string> &symbols) const;

    /**
     * Evaluate an expression. If there are still derivatives or symbols left,
     * an error is thrown
     * @return The evaluated value as a double. Other types are implicitly
     * coverted to double
     */
    double eval() const;

    /**
     * Evaluate an expression and use a fallback function if a node is
     * encountered that cannot be evaluated
     * @param fallback A function that is used if evaluation is not possible
     * @return The evaluated value as a double. Other types are implicitly
     * coverted to double
     */
    double evalDirect(std::function<double(const ExprNode &)> fallback) const;

    /**
     * Evaluate dimension nodes with given values
     * @param vals An ordered list of the coordinates in each dimension
     * @return The evaluated value as a double. Other types are implicitly
     * converted to double
     */
    double evalDims(const std::vector<double> &vals) const;

    /**
     * Replace symbols with dimension nodes
     * @param dimMap A map from dimension names to dimension indices
     */
    void replaceDims(const std::unordered_map<std::string, size_t> &dimMap);

    /**
     * Replace dimension nodes with values
     * @param vals An ordered list of the coordinates in each dimension
     */
    void replaceDims(const std::vector<double> &vals);

    /**
     * Replace variables with variable markers
     * @param nameMap A map from variable names to marker indices
     */
    void replaceVars(const std::unordered_map<std::string, size_t> &nameMap);

    /**
     * Evaluate an expression replacing variable markers with values
     * @param vals An ordered list of the values for each variable
     * @return The evaluated value as a double. Other types are implicitly
     * converted to double
     */
    double evalVars(const std::vector<double> &vals) const;

    /**
     * Check if this expression contains any variables other than "t"
     * @return A boolean indicating if any other variables were found
     */
    bool containsNonTimeVars() const;

    // The node type
    NodeType type;
    // Node children (pointers)
    std::vector<ExprNode *> children;
    // Node content (optional)
    std::string content;
    // Numeric content
    double number;
    // Index content
    size_t index;
    // Derivative information
    struct {
        // List of dimension names
        std::vector<std::string> dims;
        // Derivative amounts per dimension, which are not always present (only
        // for concrete derivatives)
        std::vector<size_t> count;
        // Variable to take derivative of
        std::string var;
    } deriv;

private:

    /**
     * Convert a binary operator to a string
     * @return A string representation of the expression
     */
    std::string binaryStr() const;

};

}