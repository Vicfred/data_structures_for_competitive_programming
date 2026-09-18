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

  static constexpr int64_t NEG_INF =
    numeric_limits<int64_t>::lowest();

  // tree[node] stores the maximum of its interval.
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

    tree[node] = max(tree[2 * node], tree[2 * node + 1]);
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

    // Recompute from both children, so the maximum
    // can increase or decrease after an assignment.
    tree[node] = max(tree[2 * node], tree[2 * node + 1]);
  }

  // Maximum of the range [query_l, query_r].
  // Time: O(log n)
  // Space: O(log n) recursion stack
  int64_t range_max(int64_t node, int64_t l, int64_t r,
                    int64_t query_l,
                    int64_t query_r) const {
    // Completely inside the query.
    if (query_l <= l && r <= query_r) {
      return tree[node];
    }

    // Completely outside: contribute the identity.
    // max(value, NEG_INF) = value.
    if (r < query_l || query_r < l) {
      return NEG_INF;
    }

    // Partial overlap: combine results from both sides.
    int64_t mid = (l + r) / 2;

    return max(
      range_max(2 * node, l, mid, query_l, query_r),
      range_max(2 * node + 1, mid + 1, r, query_l,
                query_r));
  }

public:
  // Initially represents n values equal to NEG_INF.
  // Requires n >= 1.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(int64_t size)
      : n(size), tree(4 * size, NEG_INF) {
  }

  // Build from a nonempty, 0-indexed input vector.
  // Tree operations use 1-indexed positions.
  // Time: O(n)
  // Space: O(n)
  explicit SegmentTree(const vector<int64_t> &values)
      : n(static_cast<int64_t>(values.size())),
        tree(4 * values.size(), NEG_INF) {
    build(values, 1, 1, n);
  }

  // Set element i to value; decreases are also allowed.
  // Requires 1 <= i <= n.
  // Time: O(log n)
  void update(int64_t i, int64_t value) {
    update(1, 1, n, i, value);
  }

  // Maximum of [l, r], including both endpoints.
  // Requires 1 <= l <= r <= n.
  // Time: O(log n)
  int64_t range_max(int64_t l, int64_t r) const {
    return range_max(1, 1, n, l, r);
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SegmentTree tree(values);

    assert(tree.range_max(1, 6) == 9);
    assert(tree.range_max(2, 5) == 5);
    assert(tree.range_max(3, 3) == 4);
    assert(tree.range_max(4, 6) == 9);
    assert(tree.range_max(5, 6) == 9);
  }

  {
    vector<int64_t> values = {3, 9, 4, 9, 5, 1};
    SegmentTree tree(values);

    // Decrease one of the two occurrences of 9.
    // The other still determines the overall maximum.
    tree.update(2, 2);

    assert(tree.range_max(1, 3) == 4);
    assert(tree.range_max(1, 6) == 9);
    assert(tree.range_max(2, 4) == 9);

    // Decrease the remaining 9: the maximum must fall.
    tree.update(4, 3);

    assert(tree.range_max(1, 6) == 5);
    assert(tree.range_max(2, 5) == 5);
    assert(tree.range_max(4, 6) == 5);

    // Increase a value to create a new maximum.
    tree.update(5, 12);

    assert(tree.range_max(1, 6) == 12);
    assert(tree.range_max(1, 4) == 4);
    assert(tree.range_max(5, 6) == 12);
  }

  {
    // Unset positions contribute NEG_INF.
    SegmentTree tree(5);

    assert(tree.range_max(1, 5) ==
           numeric_limits<int64_t>::lowest());

    tree.update(1, 7);
    tree.update(3, -2);
    tree.update(5, 10);

    // The represented array is now:
    // [7, NEG_INF, -2, NEG_INF, 10]
    assert(tree.range_max(1, 1) == 7);
    assert(tree.range_max(1, 2) == 7);
    assert(tree.range_max(1, 3) == 7);
    assert(tree.range_max(1, 4) == 7);
    assert(tree.range_max(1, 5) == 10);

    assert(tree.range_max(2, 2) ==
           numeric_limits<int64_t>::lowest());
    assert(tree.range_max(2, 4) == -2);
    assert(tree.range_max(4, 5) == 10);
  }

  {
    vector<int64_t> values = {5, 5, 5, 5, 5};
    SegmentTree tree(values);

    assert(tree.range_max(1, 5) == 5);
    assert(tree.range_max(2, 4) == 5);

    tree.update(1, 0);
    tree.update(5, 10);

    // The represented array is now:
    // [0, 5, 5, 5, 10]
    assert(tree.range_max(1, 1) == 0);
    assert(tree.range_max(1, 5) == 10);
    assert(tree.range_max(1, 3) == 5);
    assert(tree.range_max(4, 5) == 10);
  }

  {
    vector<int64_t> values = {-8, -3, -10, -2, -7};
    SegmentTree tree(values);

    assert(tree.range_max(1, 5) == -2);
    assert(tree.range_max(1, 2) == -3);
    assert(tree.range_max(4, 5) == -2);

    // Replace the largest value with a smaller one.
    tree.update(4, -100);

    assert(tree.range_max(1, 5) == -3);
    assert(tree.range_max(2, 4) == -3);
    assert(tree.range_max(4, 4) == -100);
  }

  {
    vector<int64_t> values = {42};
    SegmentTree tree(values);

    assert(tree.range_max(1, 1) == 42);

    tree.update(1, 2);

    assert(tree.range_max(1, 1) == 2);

    tree.update(1, 100);

    assert(tree.range_max(1, 1) == 100);
  }

  cout << "All tests passed." << endl;

  return 0;
}
