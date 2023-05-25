
#pragma once

#include "ode/spec.h"
#include <string>
#include <vector>

namespace ode::splitter {

/**
 * Main object to split up systems of ODEs
 */
class Splitter {

public:

    /**
     * Constructor
     */
    Splitter();

    /**
     * Destructor
     */
    ~Splitter();

    /**
     * Set the list of input systems to split
     * @param val The list of input systems
     */
    void setSystems(const std::vector<ODESystem> &val);

    /**
     * Run the splitter
     */
    void run();

    /**
     * Get the list of output systems (splitted input), assumes run() has been
     * called
     * @return A constant reference to the list of output systems
     */
    const std::vector<ODESystem> &getOutputSystems() const;

private:

    /**
     * Split a single system, and append result to output systems list
     * @param system The system to split
     */
    void splitSystem(const ODESystem &system);

    /**
     * Determine variable dependencies within a system
     * @param system The system to process the dependencies of
     * @return A list of lists, each sublist contains the indices of the
     * variables that the current variable depends on
     */
    std::vector<std::vector<size_t>> getDependencies(const ODESystem &system)
    const;

    /**
     * Determine the strongly connected components of a dependency graph
     * @param deps The dependencies, as returned by getDependencies()
     * @return A list of lists, each sublist contains the indices of a strongly
     * connected component. The parent list of topologically sorted
     */
    std::vector<std::vector<size_t>> getStronglyConnected(const
    std::vector<std::vector<size_t>> &deps) const;

    /**
     * Generate output systems based on a list of strongly connected components
     * @param system The system to process
     * @param comps The strongly connected components of the dependency graph
     */
    void generateFromComponents(const ODESystem &system, const
    std::vector<std::vector<size_t>> &comps);

    /**
     * Generate a unique temporary name
     * @return The unique identifier as a string
     */
    std::string getUniqueName();

    /**
     * Convert list of emits to map to vectors
     * @param emits A reference to the list of emits
     * @param emitMap The map to put the emits in
     */
    void processEmits(const std::vector<std::pair<std::string, std::string>>
    &emits, std::unordered_map<std::string, std::vector<std::string>> &emitMap)
    const;

    /**
     * Topologically sort a graph given by adjacency lists
     * @param G The graph to perform toposort on
     * @return An ordered list, the indices of the graph, toposorted
     */
    std::vector<size_t> toposort(const std::vector<std::vector<size_t>> &G)
    const;

    // List of input systems
    std::vector<ODESystem> systems;
    // List of output systems
    std::vector<ODESystem> outputSystems;
    // Counter for unique name generation
    size_t uniqueCounter;

};

}