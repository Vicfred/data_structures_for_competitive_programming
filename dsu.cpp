// vicfred
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

struct UnionFind {
private:
  vector<int64_t> parent;
  vector<int64_t> set_size;
  int64_t num_sets;

public:
  // Create n singleton sets numbered from 0 to n - 1.
  // Time: O(n)
  // Space: O(n)
  explicit UnionFind(int64_t n)
      : parent(n), set_size(n, 1), num_sets(n) {
    for (int64_t v = 0; v < n; ++v) {
      parent[v] = v;
    }
  }

  // Return the representative of v's set.
  // Path compression flattens the traversed path.
  // Time: O(alpha(n)) amortised
  int64_t find_set(int64_t v) {
    if (parent[v] == v) {
      return v;
    }

    return parent[v] = find_set(parent[v]);
  }

  // Check whether u and v belong to the same set.
  // Time: O(alpha(n)) amortised
  bool same_set(int64_t u, int64_t v) {
    return find_set(u) == find_set(v);
  }

  // Merge the sets containing u and v.
  // Time: O(alpha(n)) amortised
  void union_set(int64_t u, int64_t v) {
    int64_t x = find_set(u);
    int64_t y = find_set(v);

    if (x == y) {
      return;
    }

    // Attach the smaller component below the larger one.
    if (set_size[x] > set_size[y]) {
      swap(x, y);
    }

    parent[x] = y;
    set_size[y] += set_size[x];
    num_sets -= 1;
  }

  // Return the current number of disjoint sets.
  // Time: O(1)
  int64_t number_of_sets() const {
    return num_sets;
  }

  // Return the size of the set containing u.
  // Time: O(alpha(n)) amortised
  int64_t size_of_set(int64_t u) {
    return set_size[find_set(u)];
  }
};

int main() {
  UnionFind uf(6);

  assert(uf.number_of_sets() == 6);

  uf.union_set(0, 1);
  uf.union_set(2, 3);
  uf.union_set(3, 4);

  assert(uf.same_set(2, 4));
  assert(!uf.same_set(0, 2));

  assert(uf.size_of_set(0) == 2);
  assert(uf.size_of_set(2) == 3);
  assert(uf.number_of_sets() == 3);

  uf.union_set(1, 4);

  assert(uf.same_set(0, 2));
  assert(uf.size_of_set(3) == 5);
  assert(uf.number_of_sets() == 2);

  uf.union_set(5, 0);

  assert(uf.number_of_sets() == 1);
  assert(uf.size_of_set(5) == 6);

  cout << "All tests passed." << endl;

  return 0;
}
