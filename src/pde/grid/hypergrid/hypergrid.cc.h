
#include "hypergrid.h"
#include <algorithm>
#include <iterator>
#include <vector>

namespace pde::grid::hypergrid {

template<class T>
HyperGrid<T>::HyperGrid() : data(nullptr) { }

template<class T>
HyperGrid<T>::HyperGrid(const std::vector<size_t> &dims) : data(nullptr) {
    reshape(dims);
}

template<class T>
HyperGrid<T>::HyperGrid(const HyperGrid<T> &grid) : data(nullptr) {
    reshape(grid.getShape());
    std::copy(grid.begin(), grid.end(), begin());
}

template<class T>
HyperGrid<T>::~HyperGrid() {
    if (data != nullptr)
        delete[] data;
}

template<class T>
HyperGrid<T> &HyperGrid<T>::operator=(const HyperGrid<T> &grid) {
    reshape(grid.getShape());
    std::copy(grid.begin(), grid.end(), begin());
}

template<class T>
size_t HyperGrid<T>::size() const {
    return gridSize;
}

template<class T>
bool HyperGrid<T>::empty() const {
    return data != nullptr;
}

template<class T>
void HyperGrid<T>::clear() {
    delete[] data;
    data = nullptr;
    gridSize = 0;
}

template<class T>
void HyperGrid<T>::fill(const T &val) {
    for (T &entry : *this)
        entry = val;
}

template<class T>
void HyperGrid<T>::reshape(const std::vector<size_t> &val) {
    clear();
    dims = val;
    calcGridSize();
    data = new T[gridSize];
}

template<class T>
const std::vector<size_t> &HyperGrid<T>::getShape() const {
    return dims;
}

template<class T>
T &HyperGrid<T>::at(const std::vector<size_t> &loc) {
    return data[toIndex(loc)];
}

template<class T>
const T &HyperGrid<T>::at(const std::vector<size_t> &loc) const {
    return data[toIndex(loc)];
}

template<class T>
T &HyperGrid<T>::at(size_t index) {
    return data[index];
}

template<class T>
const T &HyperGrid<T>::at(size_t index) const {
    return data[index];
}

template<class T>
T &HyperGrid<T>::operator[](const std::vector<size_t> &loc) {
    return at(loc);
}

template<class T>
const T &HyperGrid<T>::operator[](const std::vector<size_t> &loc) const {
    return at(loc);
}

template<class T>
T &HyperGrid<T>::operator[](size_t index) {
    return at(index);
}

template<class T>
const T &HyperGrid<T>::operator[](size_t index) const {
    return at(index);
}

template<class T>
size_t HyperGrid<T>::toIndex(const std::vector<size_t> &loc) const {
    if (loc.size() != dims.size())
        throw std::runtime_error("Invalid location conversion, invalid size");
    size_t c = 0;
    for (size_t i = 0; i < loc.size(); i++) {
        c *= dims[loc.size() - i - 1];
        c += loc[loc.size() - i - 1];
    }
    if (c >= gridSize)
        throw std::runtime_error("Invalid location conversion");
    return c;
}

template<class T>
std::vector<size_t> HyperGrid<T>::toLoc(size_t index) const {
    if (index >= gridSize)
        throw std::runtime_error("Invalid index conversion, too large");
    std::vector<size_t> out(dims.size());
    for (size_t i = 0; i < dims.size(); i++) {
        out[i] = index % dims[i];
        index /= dims[i];
    }
    return out;
}

template<class T>
T *HyperGrid<T>::begin() {
    return data;
}

template<class T>
const T *HyperGrid<T>::cbegin() const {
    return data;
}

template<class T>
const T *HyperGrid<T>::begin() const {
    return cbegin();
}

template<class T>
T *HyperGrid<T>::end() {
    return data + gridSize;
}

template<class T>
const T *HyperGrid<T>::cend() const {
    return data + gridSize;
}

template<class T>
const T *HyperGrid<T>::end() const {
    return cend();
}

template<class T>
void HyperGrid<T>::calcGridSize() {
    if (data == nullptr) {
        gridSize = 0;
        return;
    }
    gridSize = 1;
    for (const size_t &val : dims)
        gridSize *= val;
}

}