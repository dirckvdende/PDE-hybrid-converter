
#pragma once

#include <vector>

namespace pde::grid {

/**
 * Basic object to represent an n-dimensional grid of data
 * @tparam T The data type to store at each grid point
 */
template<class T>
class HyperGrid {

public:

    /**
     * Constructor
     * @param dims The dimensions of the grid (size per dimension)
     */
    HyperGrid(std::vector<size_t> dims);

    /**
     * Destructor
     */
    ~HyperGrid();

    /**
     * Get the size of the grid
     * @return The total amount of points in the grid
     */
    size_t size() const;

    /**
     * Get the dimensions of the grid
     * @return A constant reference to the dimensions of the grid
     */
    const std::vector<size_t> &getDims() const;

    /**
     * Get the data at a specific location in the grid
     * @param loc The location in the grid
     * @return A reference to the data
     */
    T &get(std::vector<size_t> loc);
    const T &get(std::vector<size_t> loc) const;

    /**
     * Get data in the grid using an index
     * @param index The index of the element
     * @return A reference to the data
     */
    T &get(size_t index);
    const T &get(size_t index) const;

    /**
     * Alternative to get()
     * @param loc The location in the grid
     * @return A reference to the data at the location
     */
    T &operator[](std::vector<size_t> loc);
    const T &operator[](std::vector<size_t> loc) const;

    /**
     * Alternative to get()
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
    size_t toIndex(std::vector<size_t> loc) const;

    /**
     * Convert an index to a location in the grid (big-endian)
     * @param index The index to convert
     * @return A vector containing the converted location
     */
    std::vector<size_t> toLoc(size_t index) const;

    /**
     * Fill the grid with a given value
     * @param val The value to fill the grid with
     */
    void fill(T val);

private:

    // The dimensions of the grid
    const std::vector<size_t> dims;
    // Total amount of points in the grid
    const size_t gridSize;
    // Actual data in the grid
    T *data;

};

}

#include "hypergrid.cc.h"