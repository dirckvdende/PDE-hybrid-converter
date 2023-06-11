/**
 * PTOC: PDE to ODE converter
 * Created by Dirck van den Ende, Leiden University
 * https://github.com/dirckvdende/PDE-hybrid-converter
 */

#pragma once

#include <cstddef>
#include <iterator>
#include <vector>

namespace pde::grid::hypergrid {

/**
 * Basic object to represent an n-dimensional grid of data
 * @tparam T The data type to store at each grid point
 */
template<class T>
class HyperGrid {

public:

    /**
     * Constructor, grid is empty on initialization
     */
    HyperGrid();

    /**
     * Constructor
     * @param dims The dimensions of the grid (size per dimension)
     */
    HyperGrid(const std::vector<size_t> &dims);

    /**
     * Copy constructor
     * @param grid The other hypergrid to copy from
     */
    HyperGrid(const HyperGrid<T> &grid);

    /**
     * Destructor
     */
    virtual ~HyperGrid();

    /**
     * Assignment operator
     * @param grid The other hypergrid to copy from
     */
    HyperGrid<T> &operator=(const HyperGrid<T> &grid);

    /**
     * Get the size of the grid
     * @return The total amount of points in the grid
     */
    size_t size() const;

    /**
     * Check if the grid is empty
     * @return A boolean indicating if the grid is empty
     */
    bool empty() const;

    /**
     * Clear the entire grid, frees up the memory
     */
    virtual void clear();

    /**
     * Fill the grid with a given value
     * @param val The value to fill the grid with
     */
    virtual void fill(const T &val);

    /**
     * Change the dimensions of the grid, first clears the grid
     * @param val The new dimensions of the grid
     */
    virtual void reshape(const std::vector<size_t> &val);

    /**
     * Get the dimensions of the grid
     * @return A constant reference to the dimensions of the grid
     */
    const std::vector<size_t> &getShape() const;

    /**
     * Get the data at a specific location in the grid
     * @param loc The location in the grid
     * @return A reference to the data
     */
    T &at(const std::vector<size_t> &loc);
    const T &at(const std::vector<size_t> &loc) const;

    /**
     * Get data in the grid using an index
     * @param index The index of the element
     * @return A reference to the data
     */
    T &at(size_t index);
    const T &at(size_t index) const;

    /**
     * Alternative to at()
     * @param loc The location in the grid
     * @return A reference to the data at the location
     */
    T &operator[](const std::vector<size_t> &loc);
    const T &operator[](const std::vector<size_t> &loc) const;

    /**
     * Alternative to at()
     * @param index The index of the element
     * @return A reference to the data at the index
     */
    T &operator[](size_t index);
    const T &operator[](size_t index) const;

    /**
     * Convert a location in the grid to an index (big-endian)
     * @param loc The location in the grid
     * @return A number, the converted index
     */
    size_t toIndex(const std::vector<size_t> &loc) const;

    /**
     * Convert a pointer/reference to an index
     * @param obj The pointer/reference to convert
     * @return A number, the converted index
     */
    size_t toIndex(const T *obj) const;
    size_t toIndex(const T &obj) const;

    /**
     * Convert an index to a location in the grid (big-endian)
     * @param index The index to convert
     * @return A vector containing the converted location
     */
    std::vector<size_t> toLoc(size_t index) const;

    /**
     * Convert a pointer/reference to a location in the grid
     * @param obj The pointer/reference to convert
     * @return A vector containing the converted location
     */
    std::vector<size_t> toLoc(const T *obj) const;
    std::vector<size_t> toLoc(const T &obj) const;

    /**
     * Get a pointer to the start of the grid allocated data
     * @return A pointer to the start of the allocated data
     */
    T *begin();
    const T *cbegin() const;
    const T *begin() const;

    /**
     * Get a pointer to the end of the grid allocated data
     * @return A pointer to the end of the allocated data
     */
    T *end();
    const T *cend() const;
    const T *end() const;

private:

    /**
     * Calculate the total grid size and store the result
     */
    void calcGridSize();

    // The dimensions of the grid
    std::vector<size_t> dims;
    // Total amount of points in the grid
    size_t gridSize;
    // Actual data in the grid
    T *data;

};

}

#include "hypergrid.cc.h"