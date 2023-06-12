/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include "settings.h"
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace treeparser {

/**
 * Token type
 */
enum TokenType {
    // Normal text
    TOK_TEXT,
    // Config option name
    TOK_NAME,
    // Special characters
    TOK_LBRACE, TOK_RBRACE, TOK_SEMICOL,
};

/**
 * A lexer token is stored as a type and possible content (text)
 */
struct Token {
    TokenType type;
    std::string content;
};

/**
 * The lexer converts the input stream into tokens
 */
class Lexer {

public:

    /**
     * Constructor
     * @param txt The input text to be parsed
     */
    Lexer(std::string txt = "");

    /**
     * Destructor
     */
    ~Lexer();

    /**
     * Clear lexer input text
     */
    void clearText();

    /**
     * Append text to the lexer input text
     * @param txt The input text to be parsed
     */
    void append(std::string txt);

    /**
     * Update lexer settings
     * @param sett The new settings to be applied
     */
    void updateSettings(const Settings &sett);

    /**
     * Run the lexer, the output is stored in the lexer object
     */
    void run();

    /**
     * Get the output tokens, will return an empty vector if the lexer was not
     * run
     * @return A reference to the output token vector
     */
    const std::vector<Token> &getTokens() const;

private:

    /**
     * Get the character currently being read, or '\0' if reading past the end
     * of the input string
     * @return The current character
     */
    char cur() const;

    /**
     * Move to the next character in the input string
     */
    void next();

    /**
     * Check if the current index is past the end of the input string
     * @return A boolean indicating if current index is past input text end
     */
    bool atEnd() const;

    /**
     * Check if the last token in the tokens vector should be converted to a
     * "name" token. If it does it will be converted
     */
    void convertNameToken();

    /**
     * Convert global config options list to a map from names to config options
     */
    void prepareConfigOptions();

    /**
     * Check if the last token read was a text token
     * @return A boolean indicating if the last token was a text token
     */
    bool lastTokenWasText() const;

    /**
     * Check if a certain character is a special character
     * @param c The character to check
     * @param type Outputs the type of the character if it is a special
     * character
     * @return A boolean indicating if the given character is a special
     * character
     */
    static bool isSpecialChar(char c, TokenType &type);

    /**
     * Check if a certain character is a whitespace character
     * @param c The character to check
     * @return A boolean indicating if the given character is a whitespace
     * character
     */
    static bool isWhitespaceChar(char c);

    // Input text
    std::string txt;
    // Current character index
    size_t curIndex;
    // Output tokens
    std::vector<Token> tokens;
    // Settings
    Settings settings;
    // Config option names map
    std::unordered_map<std::string, const ConfigOption *> configNames;
    // Indicates if we are currently inside a comment
    bool inComment;

};

}