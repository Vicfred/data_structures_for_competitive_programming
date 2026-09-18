// vicfred
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct SparseTable {
private:
  // table[level][i]: minimum of the interval starting
  // at i with length 2^level.
  vector<vector<int64_t>> table;

  // logarithm[length]: largest level such that
  // 2^level <= length.
  vector<int64_t> logarithm;

public:
  // Build from a nonempty, 0-indexed input vector.
  // Requires at least one element.
  // Time: O(n log n)
  // Space: O(n log n)
  explicit SparseTable(const vector<int64_t> &values) {
    int64_t n = static_cast<int64_t>(values.size());

    logarithm.assign(n + 1, 0);
    for (int64_t length = 2; length <= n; ++length) {
      // Halving removes one power of two.
      logarithm[length] = logarithm[length / 2] + 1;
    }

    int64_t levels = logarithm[n] + 1;
    table.assign(levels, vector<int64_t>(n));

    // An interval of length one contains its own value.
    for (int64_t i = 0; i < n; ++i) {
      table[0][i] = values[i];
    }

    // Build shorter intervals before longer ones.
    for (int64_t level = 1; level < levels; ++level) {
      int64_t length = int64_t{1} << level;
      int64_t half = length / 2;

      // Only build intervals ending within the array.
      for (int64_t i = 0; i + length <= n; ++i) {
        // Combine the two consecutive equal halves.
        table[level][i] = min(table[level - 1][i],
                              table[level - 1][i + half]);
      }
    }
  }

  // Minimum over [left, right], including both ends.
  // Requires 0 <= left <= right < n.
  // Queries use the values stored at construction.
  // Time: O(1)
  int64_t range_min(int64_t left, int64_t right) const {
    int64_t length = right - left + 1;
    int64_t level = logarithm[length];
    int64_t block_length = int64_t{1} << level;

    // Choose one block starting at left and another
    // ending at right. Calculate the latter's start.
    int64_t second_start = right - block_length + 1;

    // Both blocks together cover the entire query.
    // Overlap is harmless: min(x, x) = x.
    return min(table[level][left],
               table[level][second_start]);
  }
};

int main() {
  {
    vector<int64_t> values = {3, 1, 4, 1, 5, 9};
    SparseTable table(values);

    assert(table.range_min(0, 5) == 1);
    assert(table.range_min(1, 4) == 1);
    assert(table.range_min(2, 2) == 4);
    assert(table.range_min(3, 5) == 1);
    assert(table.range_min(4, 5) == 5);
  }

  {
    vector<int64_t> values = {-8, -3, -10, -2, -7};
    SparseTable table(values);

    assert(table.range_min(0, 4) == -10);
    assert(table.range_min(0, 1) == -8);
    assert(table.range_min(1, 3) == -10);
    assert(table.range_min(3, 4) == -7);
  }

  {
    vector<int64_t> values = {42};
    SparseTable table(values);

    assert(table.range_min(0, 0) == 42);
  }

  cout << "All tests passed." << endl;

  return 0;
}
