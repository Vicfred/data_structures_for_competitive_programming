// vicfred
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct SegmentTree {
private:
  int64_t n;

  static constexpr int64_t INF =
    numeric_limits<int64_t>::max();

  // tree[node] stores the minimum of its interval.
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

    tree[node] = min(tree[2 * node], tree[2 * node + 1]);
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

    // Recompute from both children, so the minimum
    // can increase or decrease after an assignment.
    tree[node] = min(tree[2 * node], tree[2 * node + 1]);
  }

  // Minimum of the range [query_l, query_r].
  // Time: O(log n)
  // Space: O(log n) recursion stack
  int64_t range_min(int64_t node, int64_t l, int64_t r,
                    int64_t query_l,
                    int64_t query_r) const {
    // Completely inside the query.
    if (query_l <= l && r <= query_r) {
      return tree[node];
    }

    // Completely outside: contribute the identity.
    // min(value, INF) = value.
    if (r < query_l || query_r < l) {
      return INF;
    }

    // Partial overlap: combine results from both sides.
    int64_t mid = (l + r) / 2;

    return min(
      range_min(2 * node, l, mid, query_l, query_r),
      range_min(2 * node + 1, mid + 1, r, query_l,
                query_r));
  }

public:
  // Initially represents n values equal to INF.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(int64_t size)
      : n(size), tree(4 * size, INF) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(const vector<int64_t> &values)
      : n(static_cast<int64_t>(values.size())),
        tree(4 * values.size(), INF) {
    build(values, 1, 1, n);
  }

  // Set element i to value; increases are also allowed.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t value) {
    update(1, 1, n, i, value);
  }

  // Minimum of [l, r], including both endpoints.
  // Requires 1 <= l <= r <= n.
  // Time: O(log n)
  int64_t range_min(int64_t l, int64_t r) const {
    return range_min(1, 1, n, l, r);
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SegmentTree tree(values);

    assert(tree.range_min(1, 6) == 1);
    assert(tree.range_min(2, 5) == 1);
    assert(tree.range_min(3, 3) == 4);
    assert(tree.range_min(4, 6) == 1);
    assert(tree.range_min(5, 6) == 5);
  }

  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SegmentTree tree(values);

    // Increase one of the two occurrences of 1.
    // The other still determines the overall minimum.
    tree.update(2, 8);

    assert(tree.range_min(1, 3) == 3);
    assert(tree.range_min(1, 6) == 1);
    assert(tree.range_min(2, 4) == 1);

    // Increase the remaining 1: the minimum must rise.
    tree.update(4, 7);

    assert(tree.range_min(1, 6) == 3);
    assert(tree.range_min(2, 5) == 4);
    assert(tree.range_min(4, 6) == 5);

    // Decrease a value to create a new minimum.
    tree.update(5, -2);

    assert(tree.range_min(1, 6) == -2);
    assert(tree.range_min(1, 4) == 3);
    assert(tree.range_min(5, 6) == -2);
  }

  {
    // Unset positions contribute INF, rather than zero.
    SegmentTree tree(5);

    assert(tree.range_min(1, 5) ==
           numeric_limits<int64_t>::max());

    tree.update(1, 7);
    tree.update(3, -2);
    tree.update(5, 10);

    // The represented array is now:
    // [7, INF, -2, INF, 10]
    assert(tree.range_min(1, 1) == 7);
    assert(tree.range_min(1, 2) == 7);
    assert(tree.range_min(1, 3) == -2);
    assert(tree.range_min(1, 4) == -2);
    assert(tree.range_min(1, 5) == -2);

    assert(tree.range_min(2, 2) ==
           numeric_limits<int64_t>::max());
    assert(tree.range_min(2, 4) == -2);
    assert(tree.range_min(4, 5) == 10);
  }

  {
    vector<int64_t> values = {5, 5, 5, 5, 5};
    SegmentTree tree(values);

    assert(tree.range_min(1, 5) == 5);
    assert(tree.range_min(2, 4) == 5);

    tree.update(1, 0);
    tree.update(5, 10);

    // The represented array is now:
    // [0, 5, 5, 5, 10]
    assert(tree.range_min(1, 1) == 0);
    assert(tree.range_min(1, 5) == 0);
    assert(tree.range_min(1, 3) == 0);
    assert(tree.range_min(4, 5) == 5);
  }

  {
    vector<int64_t> values = {-8, -3, -10, -2, -7};
    SegmentTree tree(values);

    assert(tree.range_min(1, 5) == -10);
    assert(tree.range_min(1, 2) == -8);
    assert(tree.range_min(4, 5) == -7);

    // Replace the smallest value with a larger one.
    tree.update(3, 100);

    assert(tree.range_min(1, 5) == -8);
    assert(tree.range_min(2, 4) == -3);
    assert(tree.range_min(3, 3) == 100);
  }

  {
    vector<int64_t> values = {42};
    SegmentTree tree(values);

    assert(tree.range_min(1, 1) == 42);

    tree.update(1, 2);

    assert(tree.range_min(1, 1) == 2);

    tree.update(1, 100);

    assert(tree.range_min(1, 1) == 100);
  }

  cout << "All tests passed." << endl;

  return 0;
}
