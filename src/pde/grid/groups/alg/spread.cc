
#include "pde/grid/groups/groupgrid.h"
#include "spread.h"
#include <queue>

using namespace pde::grid::groups::alg;

SpreadAlg::~SpreadAlg() { }

void SpreadAlg::run() {
    // Keep track of nodes that have been grouped already
    std::vector<bool> grouped(grid->size(), false);
    for (size_t i = 0; i < grid->size(); i++) {
        if (grouped[i])
            continue;
        std::queue<size_t> q;
        q.push(i);
        grouped[i] = true;
        std::vector<size_t> base = grid->toLoc(i);
        while (!q.empty()) {
            std::vector<size_t> cur = grid->toLoc(q.front());
            q.pop();
            for (size_t d = 0; d < grid->getShape().size(); d++) {
                if (cur[d] > 0) {
                    cur[d]--;
                    size_t j = grid->toIndex(cur);
                    if (grid->canJoin(base, cur) && !grouped[j]) {
                        grid->join(base, cur);
                        grouped[j] = true;
                        q.push(j);
                    }
                    cur[d]++;
                }
                if (cur[d] + 1 < grid->getShape()[d]) {
                    cur[d]++;
                    size_t j = grid->toIndex(cur);
                    if (grid->canJoin(base, cur) && !grouped[j]) {
                        grid->join(base, cur);
                        grouped[j] = true;
                        q.push(j);
                    }
                    cur[d]--;
                }
            }
        }
    }
}