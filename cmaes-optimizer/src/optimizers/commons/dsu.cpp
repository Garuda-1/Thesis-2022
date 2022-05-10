#include "dsu.h"

size_t common::dsu::find_set(size_t x) {
  if (x == parent[x]) {
    return x;
  }
  return parent[x] = find_set(parent[x]);
}

void common::dsu::union_sets(size_t x, size_t y) {
  x = find_set(x);
  y = find_set(y);
  if (x != y) {
    if (rank[x] > rank[y]) {
      std::swap(x, y);
    }
    parent[x] = y;
    if (rank[x] == rank[y]) {
      ++rank[y];
    }
  }
}