
#include "hypergrid.h"
#include <vector>
#include <stdexcept>
#include <numeric>

namespace pde::grid::hypergrid {

template<class T>
HyperGrid<T>::HyperGrid(std::vector<size_t> dims) : dims(dims),
gridSize(std::accumulate(dims.begin(), dims.end(), 1UL,
std::multiplies<size_t>())), data(new T[gridSize]) { }

template<class T>
HyperGrid<T>::~HyperGrid() {
    delete[] data;
}

template<class T>
size_t HyperGrid<T>::size() const {
    return gridSize;
}

template<class T>
const std::vector<size_t> &HyperGrid<T>::getDims() const {
    return dims;
}

template<class T>
T &HyperGrid<T>::get(std::vector<size_t> loc) {
    return data[toIndex(loc)];
}

template<class T>
const T &HyperGrid<T>::get(std::vector<size_t> loc) const {
    return data[toIndex(loc)];
}

template<class T>
T &HyperGrid<T>::get(size_t index) {
    if (index >= gridSize)
        throw std::runtime_error("Given index too large");
    return data[index];
}

template<class T>
const T &HyperGrid<T>::get(size_t index) const {
    if (index >= gridSize)
        throw std::runtime_error("Given index too large");
    return data[index];
}

template<class T>
T &HyperGrid<T>::operator[](std::vector<size_t> loc) {
    return get(loc);
}

template<class T>
const T &HyperGrid<T>::operator[](std::vector<size_t> loc) const {
    return get(loc);
}

template<class T>
T &HyperGrid<T>::operator[](size_t index) {
    return get(index);
}

template<class T>
const T &HyperGrid<T>::operator[](size_t index) const {
    return get(index);
}

template<class T>
size_t HyperGrid<T>::toIndex(std::vector<size_t> loc) const {
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
void HyperGrid<T>::fill(T val) {
    for (size_t i = 0; i < gridSize; i++)
        data[i] = val;
}

}