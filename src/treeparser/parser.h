/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <vector>
#include "lexer.h"

namespace treeparser {

/**
 * Type of a parse node
 * If the type is text, then only text is stored along with the name
 * If the type is tree, only a list of children is stored along with the name
 * If the type is named tree, both are stored along with the name
 * The root type is only used for the root of the tree, which is a tree without
 * a name
 */
enum ParseNodeType {
    NODE_TEXT,
    NODE_TREE,
    NODE_NAMED_TREE,
    NODE_ROOT,
};

/**
 * A node in the parse tree
 * Contains the type of the parse node, and possibly text and child pointers
 */
struct ParseNode {
    ParseNode(const ParseNodeType type);
    ~ParseNode();
    std::ostream &print(std::ostream &os, size_t depth);
    const ParseNodeType type;
    std::string name, text;
    std::vector<ParseNode *> children;
};

/**
 * The parser converts the list of tokens generated by the lexer to a tree
 */
class Parser {

public:

    /**
     * Constructor
     */
    Parser();

    /**
     * Destructor
     */
    ~Parser();

    /**
     * Set the input text of the parser
     * @param txt The input text
     */
    void setText(std::string txt);

    /**
     * Run the parser, the generated parse tree is stored in this parser object
     */
    void run();

    /**
     * Adjust config settings
     * @param sett The updated settings
     */
    void updateSettings(const Settings &sett);

    /**
     * Get the root node of the parse tree
     * @return A pointer to the parse tree root node
     */
    ParseNode *getRoot() const;

private:

    /**
     * Get the current token
     * @return A constant reference to the current token, of the last token if
     * reading past the end of the input
     */
    const Token &cur() const;

    /**
     * Move to the next token in the input list
     */
    void next();

    /**
     * Check if we are reading past the end of the input list
     * @return A boolean indicating if the current index is past the end of the
     * input list
     */
    bool atEnd() const;

    /**
     * Check if the type of the current token matches the given type
     * @param type The type to match with
     * @return A boolean indicating if the types match
     */
    bool accept(TokenType type) const;

    /**
     * Check if the type of the current token matches the given type, if it does
     * not throw an error
     * @param type The type to match with
     */
    void expect(TokenType type);

    /**
     * Show a parse error
     * @param content The text to display with the error
     */
    static void error(std::string content);

    /**
     * Read in the root node of the parse node
     * @return A pointer to the generated parse node
     */
    ParseNode *readRoot();

    /**
     * Read in a named configuration
     * @return A pointer to the generated parse node
     */
    ParseNode *readConfigEntry();

    /**
     * Read in a named text entry
     * @return A pointer to the generated parse node
     */
    ParseNode *readTextEntry();

    /**
     * Read in a named tree entry
     * @param isNamed if the tree entry is named
     * @return A pointer to the generated parse node
     */
    ParseNode *readTreeEntry(bool isNamed);

    /**
     * Convert config options list to a map from names to config options
     */
    void prepareConfigOptions();

    // Tree settings
    Settings settings;
    // Root of the generated parse tree, null if not run yet
    ParseNode *root;
    // Current reading index in the tokens vector
    size_t curIndex;
    // Config option names map
    std::unordered_map<std::string, const ConfigOption *> configNames;
    // Tree lexer
    Lexer lexer;
    // Reference to the list of tokens generated by the lexer
    const std::vector<Token> &tokens;

};

}