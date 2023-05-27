
#include "dsu.h"

using namespace pde::grid::groups;

DisjointUnion::DisjointUnion(size_t n) : parent(n), sizes(n, 1) {
    for (size_t i = 0; i < n; i++)
        parent[i] = i;
}

DisjointUnion::~DisjointUnion() { }

size_t DisjointUnion::find(size_t x) {
    if (parent[x] == x)
        return x;
    // This is an optimization to improve future searches
    parent[x] = find(parent[x]);
    return parent[x];
}

void DisjointUnion::join(size_t x, size_t y) {
    x = find(x);
    y = find(y);
    // x and y are equal
    if (x == y)
        return;
    // After this x will have the most descendents
    // This is to keep the tree as balanced as possible
    if (sizes[x] < sizes[y])
        std::swap(sizes[x], sizes[y]);
    parent[y] = x;
    sizes[x] = sizes[x] + sizes[y];
}

size_t DisjointUnion::groupSize(size_t x) {
    return sizes[find(x)];
}