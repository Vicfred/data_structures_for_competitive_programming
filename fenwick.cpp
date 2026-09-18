// vicfred
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct FenwickTree {
private:
  // 1-indexed Fenwick tree.
  vector<int64_t> fenwick;

public:
  // Initially represents n values equal to zero.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(int64_t n)
      : fenwick(n + 1) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit FenwickTree(const vector<int64_t> &values)
      : fenwick(values.size() + 1) {
    int64_t n = static_cast<int64_t>(values.size());

    for (int64_t i = 1; i <= n; ++i) {
      int64_t parent = i + (i & -i);

      fenwick[i] += values[i - 1];

      if (parent <= n) {
        fenwick[parent] += fenwick[i];
      }
    }
  }

  // Add delta to the value at position i.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t delta) {
    while (i < static_cast<int64_t>(fenwick.size())) {
      fenwick[i] += delta;
      i += i & -i;
    }
  }

  // Sum of the prefix [1, i].
  // Requires 0 <= i <= n.
  // Time: O(log n)
  int64_t prefix_sum(int64_t i) const {
    int64_t sum = 0;

    while (i > 0) {
      sum += fenwick[i];
      i -= i & -i;
    }

    return sum;
  }

  // Sum of [l, r], including both endpoints.
  // Requires 1 <= l <= r <= n.
  // Time: O(log n)
  int64_t range_sum(int64_t l, int64_t r) const {
    return prefix_sum(r) - prefix_sum(l - 1);
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_sum(1) == 3);
    assert(fenwick.prefix_sum(2) == 4);
    assert(fenwick.prefix_sum(3) == 8);
    assert(fenwick.prefix_sum(4) == 9);
    assert(fenwick.prefix_sum(5) == 14);
    assert(fenwick.prefix_sum(6) == 23);

    assert(fenwick.range_sum(1, 6) == 23);
    assert(fenwick.range_sum(2, 5) == 11);
    assert(fenwick.range_sum(3, 3) == 4);
    assert(fenwick.range_sum(4, 6) == 15);
  }

  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    FenwickTree fenwick(values);

    // Change the third value from 4 to 10.
    fenwick.update(3, 6);

    assert(fenwick.prefix_sum(3) == 14);
    assert(fenwick.prefix_sum(6) == 29);
    assert(fenwick.range_sum(2, 4) == 12);

    // Change the fifth value from 5 to 2.
    fenwick.update(5, -3);

    assert(fenwick.prefix_sum(5) == 17);
    assert(fenwick.prefix_sum(6) == 26);
    assert(fenwick.range_sum(5, 6) == 11);
  }

  {
    // Build an initially empty Fenwick tree.
    FenwickTree fenwick(5);

    fenwick.update(1, 7);
    fenwick.update(3, -2);
    fenwick.update(5, 10);

    // The represented array is now:
    // [7, 0, -2, 0, 10]
    assert(fenwick.prefix_sum(1) == 7);
    assert(fenwick.prefix_sum(2) == 7);
    assert(fenwick.prefix_sum(3) == 5);
    assert(fenwick.prefix_sum(4) == 5);
    assert(fenwick.prefix_sum(5) == 15);

    assert(fenwick.range_sum(2, 4) == -2);
    assert(fenwick.range_sum(3, 5) == 8);
  }

  {
    vector<int64_t> values = {5, 5, 5, 5, 5};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_sum(5) == 25);
    assert(fenwick.range_sum(2, 4) == 15);

    fenwick.update(1, -5);
    fenwick.update(5, 5);

    // The represented array is now:
    // [0, 5, 5, 5, 10]
    assert(fenwick.prefix_sum(1) == 0);
    assert(fenwick.prefix_sum(5) == 25);
    assert(fenwick.range_sum(1, 3) == 10);
    assert(fenwick.range_sum(4, 5) == 15);
  }

  {
    vector<int64_t> values = {42};
    FenwickTree fenwick(values);

    assert(fenwick.prefix_sum(1) == 42);
    assert(fenwick.range_sum(1, 1) == 42);

    fenwick.update(1, -40);

    assert(fenwick.prefix_sum(1) == 2);
    assert(fenwick.range_sum(1, 1) == 2);
  }

  cout << "All tests passed." << endl;

  return 0;
}
