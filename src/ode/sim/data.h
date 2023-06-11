/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace ode::sim {

/**
 * Object used to keep track of variable names and emit variables
 */
class SimData {

public:

    /**
     * Constructor
     */
    SimData();

    /**
     * Destructor
     */
    ~SimData();

    /**
     * Add a variable to the dataframe. If the variable already exists, it will
     * be reset
     * @param name The name of the variable
     */
    void add(const std::string &name);

    /**
     * Get data at a specific index of the data frame
     * @param index The index in the data frame
     * @param step The time step at which the data should be retrieved
     * @return A double with the 
     */
    double get(size_t index, size_t step) const;

    /**
     * Get the number of time steps recorded for a specific index
     * @param index The index in the data frame
     * @return The number of time steps recorded
     */
    size_t size(size_t index) const;

    /**
     * Get the index of a variable in the data frame
     * @param name The name of the variable
     * @return The index of the variable
     */
    size_t getIndex(const std::string &name) const;

    /**
     * Append data to a speicifc index in the data frame
     * @param index The index at which to append
     * @param val The value to append
     */
    void append(size_t index, double val);

    /**
     * Reset data frame variables that are not emit variables
     */
    void reset();

    /**
     * Add an emit to the dataframe
     * @param inp Input variable name
     * @param out Output variable name
     */
    void addEmit(const std::string &inp, const std::string &out);

    /**
     * Get all emit names and indices, filtering out emits that start with '_'
     * @return A list of pairs, containing the names and indices of the emits
     */
    std::vector<std::pair<std::string, size_t>> emitList() const;

private:

    /**
     * Data frame entry
     */
    struct DataEntry {
        // Entry has one or more emit references
        size_t emitRefs = 0;
        // Data values
        std::vector<double> values;
    };

    // Map from variable names to indices in the data frame
    std::unordered_map<std::string, size_t> names;
    // Emit variable indices
    std::unordered_map<std::string, size_t> emitNames;
    // Data frame data
    std::vector<DataEntry> data;

};

}