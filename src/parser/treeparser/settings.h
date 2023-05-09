
#ifndef TREEPARSER_SETTINGS_H
#define TREEPARSER_SETTINGS_H

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
enum ConfigOptionType {
    OPT_TEXT,
    OPT_TREE,
    OPT_NAMED_TREE,
};

/**
 * Possible configuration options
 * Name gives the identifier that the option starts with, type gives the
 * type (see above)
 */
struct ConfigOption {
    std::string name;
    ConfigOptionType type = OPT_TEXT;
};

// Stores all configuration options
extern const std::vector<ConfigOption> configOptions;

/**
 * Contains all global settings
 */
struct Settings {
    std::vector<ConfigOption> configOptions;
};

extern const Settings settings;

#endif