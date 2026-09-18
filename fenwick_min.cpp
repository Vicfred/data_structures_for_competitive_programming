// vicfred
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct FenwickTree {
private:
  // 1-indexed Fenwick tree storing prefix minima.
  vector<int64_t> fenwick;

  static constexpr int64_t INF =
    numeric_limits<int64_t>::max();

public:
  // Initially represents n values equal to INF.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(int64_t n)
      : fenwick(n + 1, INF) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(const vector<int64_t> &values)
      : fenwick(values.size() + 1, INF) {
    int64_t n = static_cast<int64_t>(values.size());

    for (int64_t i = 1; i <= n; ++i) {
      int64_t parent = i + (i & -i);

      fenwick[i] = min(fenwick[i], values[i - 1]);

      if (parent <= n) {
        fenwick[parent] = min(fenwick[parent], fenwick[i]);
      }
    }
  }

  // Make the value represented at i at most value.
  // This operation only supports decreasing minima.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t value) {
    while (i < static_cast<int64_t>(fenwick.size())) {
      fenwick[i] = min(fenwick[i], value);
      i += i & -i;
    }
  }

  // Minimum over the prefix [1, i].
  // Requires 0 <= i <= n.
  // Time: O(log n)
  int64_t prefix_min(int64_t i) const {
    int64_t result = INF;

    while (i > 0) {
      result = min(result, fenwick[i]);
      i -= i & -i;
    }

    return result;
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_min(1) == 3);
    assert(fenwick.prefix_min(2) == 1);
    assert(fenwick.prefix_min(3) == 1);
    assert(fenwick.prefix_min(4) == 1);
    assert(fenwick.prefix_min(5) == 1);
    assert(fenwick.prefix_min(6) == 1);
  }

  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    // Decrease position 3 from 4 to -10.
    fenwick.update(3, -10);

    assert(fenwick.prefix_min(2) == 1);
    assert(fenwick.prefix_min(3) == -10);
    assert(fenwick.prefix_min(6) == -10);

    // Decrease position 5 from 5 to -20.
    fenwick.update(5, -20);

    assert(fenwick.prefix_min(4) == -10);
    assert(fenwick.prefix_min(5) == -20);
    assert(fenwick.prefix_min(6) == -20);
  }

  {
    // Build an initially empty Fenwick tree.
    FenwickTree fenwick(5);

    fenwick.update(1, 7);
    fenwick.update(3, 2);
    fenwick.update(5, 10);

    assert(fenwick.prefix_min(1) == 7);
    assert(fenwick.prefix_min(2) == 7);
    assert(fenwick.prefix_min(3) == 2);
    assert(fenwick.prefix_min(4) == 2);
    assert(fenwick.prefix_min(5) == 2);
  }

  {
    vector<int64_t> values = {-8, -3, -10, -2, -7};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_min(1) == -8);
    assert(fenwick.prefix_min(2) == -8);
    assert(fenwick.prefix_min(3) == -10);
    assert(fenwick.prefix_min(4) == -10);
    assert(fenwick.prefix_min(5) == -10);

    // Decrease position 3 from -10 to -100.
    fenwick.update(3, -100);

    assert(fenwick.prefix_min(2) == -8);
    assert(fenwick.prefix_min(3) == -100);
    assert(fenwick.prefix_min(5) == -100);
  }

  {
    vector<int64_t> values = {42};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_min(1) == 42);

    fenwick.update(1, -100);

    assert(fenwick.prefix_min(1) == -100);
  }

  cout << "All tests passed." << endl;

  return 0;
}
