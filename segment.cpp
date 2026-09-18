// vicfred
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct SegmentTree {
private:
  int64_t n;

  // tree[node] stores the sum of its interval.
  vector<int64_t> tree;

  // Build the whole tree.
  // Time: O(n)
  // Space: O(log n) recursion stack
  void build(const vector<int64_t> &values, int64_t node,
             int64_t l, int64_t r) {
    if (l == r) {
      tree[node] = values[l - 1];
      return;
    }

    int64_t mid = (l + r) / 2;

    build(values, 2 * node, l, mid);
    build(values, 2 * node + 1, mid + 1, r);

    tree[node] = tree[2 * node] + tree[2 * node + 1];
  }

  // Set element i to value.
  // Time: O(log n)
  // Space: O(log n) recursion stack
  void update(int64_t node, int64_t l, int64_t r, int64_t i,
              int64_t value) {
    if (l == r) {
      tree[node] = value;
      return;
    }

    int64_t mid = (l + r) / 2;

    if (i <= mid) {
      update(2 * node, l, mid, i, value);
    } else {
      update(2 * node + 1, mid + 1, r, i, value);
    }

    // Recompute this interval after changing
    // one of its children.
    tree[node] = tree[2 * node] + tree[2 * node + 1];
  }

  // Sum of the range [query_l, query_r].
  // Time: O(log n)
  // Space: O(log n) recursion stack
  int64_t range_sum(int64_t node, int64_t l, int64_t r,
                    int64_t query_l,
                    int64_t query_r) const {
    // Completely inside the query.
    if (query_l <= l && r <= query_r) {
      return tree[node];
    }

    // Completely outside the query.
    if (r < query_l || query_r < l) {
      return 0;
    }

    // Partial overlap: query both children.
    int64_t mid = (l + r) / 2;

    return range_sum(2 * node, l, mid, query_l, query_r) +
           range_sum(2 * node + 1, mid + 1, r, query_l,
                     query_r);
  }

public:
  // Initially represents n values equal to zero.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(int64_t size)
      : n(size), tree(4 * size) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(const vector<int64_t> &values)
      : n(static_cast<int64_t>(values.size())),
        tree(4 * values.size()) {
    build(values, 1, 1, n);
  }

  // Set the value at position i.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t value) {
    update(1, 1, n, i, value);
  }

  // Sum of [l, r], including both endpoints.
  // Requires 1 <= l <= r <= n.
  // Time: O(log n)
  int64_t range_sum(int64_t l, int64_t r) const {
    return range_sum(1, 1, n, l, r);
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SegmentTree tree(values);

    assert(tree.range_sum(1, 6) == 23);
    assert(tree.range_sum(2, 5) == 11);
    assert(tree.range_sum(3, 3) == 4);
    assert(tree.range_sum(4, 6) == 15);
  }

  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SegmentTree tree(values);

    // Change the third value from 4 to 10.
    tree.update(3, 10);

    assert(tree.range_sum(1, 3) == 14);
    assert(tree.range_sum(1, 6) == 29);
    assert(tree.range_sum(2, 4) == 12);

    // Change the fifth value from 5 to 2.
    tree.update(5, 2);

    assert(tree.range_sum(1, 5) == 17);
    assert(tree.range_sum(1, 6) == 26);
    assert(tree.range_sum(5, 6) == 11);
  }

  {
    // Build an initially empty segment tree.
    SegmentTree tree(5);

    tree.update(1, 7);
    tree.update(3, -2);
    tree.update(5, 10);

    // The represented array is now:
    // [7, 0, -2, 0, 10]
    assert(tree.range_sum(1, 1) == 7);
    assert(tree.range_sum(1, 2) == 7);
    assert(tree.range_sum(1, 3) == 5);
    assert(tree.range_sum(1, 4) == 5);
    assert(tree.range_sum(1, 5) == 15);

    assert(tree.range_sum(2, 4) == -2);
    assert(tree.range_sum(3, 5) == 8);
  }

  {
    vector<int64_t> values = {5, 5, 5, 5, 5};
    SegmentTree tree(values);

    assert(tree.range_sum(1, 5) == 25);
    assert(tree.range_sum(2, 4) == 15);

    tree.update(1, 0);
    tree.update(5, 10);

    // The represented array is now:
    // [0, 5, 5, 5, 10]
    assert(tree.range_sum(1, 1) == 0);
    assert(tree.range_sum(1, 5) == 25);
    assert(tree.range_sum(1, 3) == 10);
    assert(tree.range_sum(4, 5) == 15);
  }

  {
    vector<int64_t> values = {42};
    SegmentTree tree(values);

    assert(tree.range_sum(1, 1) == 42);

    tree.update(1, 2);

    assert(tree.range_sum(1, 1) == 2);
  }

  cout << "All tests passed." << endl;

  return 0;
}
