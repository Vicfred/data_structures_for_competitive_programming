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
  // 1-indexed Fenwick tree storing prefix maxima.
  vector<int64_t> fenwick;

  static constexpr int64_t NEG_INF =
    numeric_limits<int64_t>::lowest();

public:
  // Initially represents n values equal to NEG_INF.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(int64_t n)
      : fenwick(n + 1, NEG_INF) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(const vector<int64_t> &values)
      : fenwick(values.size() + 1, NEG_INF) {
    int64_t n = static_cast<int64_t>(values.size());

    for (int64_t i = 1; i <= n; ++i) {
      int64_t parent = i + (i & -i);

      fenwick[i] = max(fenwick[i], values[i - 1]);

      if (parent <= n) {
        fenwick[parent] = max(fenwick[parent], fenwick[i]);
      }
    }
  }

  // Make the value represented at i at least value.
  // This operation only supports increasing maxima.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t value) {
    while (i < static_cast<int64_t>(fenwick.size())) {
      fenwick[i] = max(fenwick[i], value);
      i += i & -i;
    }
  }

  // Maximum over the prefix [1, i].
  // Requires 0 <= i <= n.
  // Time: O(log n)
  int64_t prefix_max(int64_t i) const {
    int64_t result = NEG_INF;

    while (i > 0) {
      result = max(result, fenwick[i]);
      i -= i & -i;
    }

    return result;
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_max(1) == 3);
    assert(fenwick.prefix_max(2) == 3);
    assert(fenwick.prefix_max(3) == 4);
    assert(fenwick.prefix_max(4) == 4);
    assert(fenwick.prefix_max(5) == 5);
    assert(fenwick.prefix_max(6) == 9);
  }

  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    // Increase position 3 from 4 to 10.
    fenwick.update(3, 10);

    assert(fenwick.prefix_max(2) == 3);
    assert(fenwick.prefix_max(3) == 10);
    assert(fenwick.prefix_max(6) == 10);

    // Increase position 5 from 5 to 20.
    fenwick.update(5, 20);

    assert(fenwick.prefix_max(4) == 10);
    assert(fenwick.prefix_max(5) == 20);
    assert(fenwick.prefix_max(6) == 20);
  }

  {
    // Build an initially empty Fenwick tree.
    FenwickTree fenwick(5);

    fenwick.update(1, 7);
    fenwick.update(3, 2);
    fenwick.update(5, 10);

    assert(fenwick.prefix_max(1) == 7);
    assert(fenwick.prefix_max(2) == 7);
    assert(fenwick.prefix_max(3) == 7);
    assert(fenwick.prefix_max(4) == 7);
    assert(fenwick.prefix_max(5) == 10);
  }

  {
    vector<int64_t> values = {-8, -3, -10, -2, -7};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_max(1) == -8);
    assert(fenwick.prefix_max(2) == -3);
    assert(fenwick.prefix_max(3) == -3);
    assert(fenwick.prefix_max(4) == -2);
    assert(fenwick.prefix_max(5) == -2);

    fenwick.update(3, 100);

    assert(fenwick.prefix_max(2) == -3);
    assert(fenwick.prefix_max(3) == 100);
    assert(fenwick.prefix_max(5) == 100);
  }

  {
    vector<int64_t> values = {42};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_max(1) == 42);

    fenwick.update(1, 100);

    assert(fenwick.prefix_max(1) == 100);
  }

  cout << "All tests passed." << endl;

  return 0;
}
