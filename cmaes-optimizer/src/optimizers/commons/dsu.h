#ifndef OPTIMIZER_DSU_H
#define OPTIMIZER_DSU_H

#include <vector>

namespace common {

struct dsu {
  std::vector<size_t> parent, rank;

  explicit dsu(size_t n) : parent(n), rank(n) {
    for (size_t i = 0; i < n; ++i) {
      parent[i] = i;
    }
  }

  size_t find_set(size_t x);

  void union_sets(size_t x, size_t y);
};

}  // namespace common

#endif
