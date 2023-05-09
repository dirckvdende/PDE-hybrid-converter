
#pragma once

#include <vector>
#include <string>

/**
 * Configuration option types
 * If the type is text, then text is expected with a semicolon afterwards
 * If the type is tree, then a subtree enclosed by {} is expected (semicolon
 * optional)
 * If the type is named tree, then a name is expected first, then a subtree
 * enclosed by {} (semicolon optional)
 */
enum TreeConfigOptionType {
    OPT_TEXT,
    OPT_TREE,
    OPT_NAMED_TREE,
};

/**
 * Possible configuration options
 * Name gives the identifier that the option starts with, type gives the
 * type (see above)
 */
struct TreeConfigOption {
    std::string name;
    TreeConfigOptionType type = OPT_TEXT;
};

// Stores all configuration options
extern const std::vector<TreeConfigOption> configOptions;

/**
 * Contains all global settings
 */
struct TreeSettings {
    std::vector<TreeConfigOption> configOptions;
};

extern const TreeSettings settings;