# Data Structures

A small C++23 reference collection of fundamental and
interesting data structures. The implementations favour clarity,
explicit invariants, and useful tests over terse contest-style code.

Each `.cpp` file is self-contained and includes an assertion-based
test programme, so it can be compiled and run independently.

## Contents

| File | Data structure | Main operations | Complexity |
| --- | --- | --- | --- |
| `dsu.cpp` | Disjoint-set union | `find_set`, `same_set`, `union_set`, component size and count | `O(α(n))` amortised per search or merge |
| `fenwick.cpp` | Fenwick tree for sums | Point addition, prefix sum, range sum | `O(log n)` per update or query |
| `fenwick_min.cpp` | Fenwick tree for prefix minima | Decreasing point update, prefix minimum | `O(log n)` per update or query |
| `fenwick_max.cpp` | Fenwick tree for prefix maxima | Increasing point update, prefix maximum | `O(log n)` per update or query |
| `segment.cpp` | Segment tree for sums | Point assignment, range sum | `O(log n)` per update or query |
| `segment_min.cpp` | Segment tree for minima | Point assignment, range minimum | `O(log n)` per update or query |
| `segment_max.cpp` | Segment tree for maxima | Point assignment, range maximum | `O(log n)` per update or query |
| `sparse.cpp` | Sparse table for static minima | Immutable range minimum | `O(1)` per query after `O(n log n)` construction |
| `treap.cpp` | Randomised binary search tree | Insert, erase, membership, sorted traversal | Expected `O(log n)` per update or search |
| `trie.cpp` | Trie for lowercase English words | Insert, exact membership, prefix membership | `O(L)`, where `L` is the string length |
| `fibonacci.cpp` | Fibonacci min-heap | Insert, minimum, extract minimum, decrease key, meld | Operation-dependent amortised bounds |

## Conventions

- C++23 with standard-library headers rather than
  `bits/stdc++.h`.
- Two-space indentation and K&R braces.
- `snake_case` for functions and variables.
- `int64_t` for integer values and indices.
- Public constructors are `explicit` where appropriate.
- Comments state indexing, preconditions, and complexities.
- Tests print `All tests passed.` when successful.

### Indexing

- Disjoint-set elements are numbered from `0` to `n - 1`.
- Fenwick-tree and segment-tree operations are 1-indexed.
- Their vector constructors accept ordinary 0-indexed vectors.
- Sparse-table construction and queries are 0-indexed.
- Trie nodes and treap internals are implementation details.

These choices are intentional. In particular, 1-indexing makes the
Fenwick-tree bit operations and the recursive segment-tree intervals
especially natural, while 0-indexing suits the sparse-table blocks.

## Important limitations

### Fenwick trees

The sum variant supports arbitrary additive changes. The specialised
minimum and maximum variants are monotonic:

- `fenwick_min.cpp` only supports updates that decrease a value.
- `fenwick_max.cpp` only supports updates that increase a value.

They cannot undo an old minimum or maximum. Use the corresponding
segment tree when arbitrary assignments are required.

### Sparse table

The sparse table is immutable after construction. Its constant-time
query works for minimum because minimum is idempotent: overlapping
blocks do not change the answer.

### Treap

The treap behaves as a set, not a multiset. Inserting an existing key
does nothing. Its logarithmic bounds are expected rather than
worst-case because priorities are random.

### Trie

The trie accepts only lowercase English letters from `a` through `z`.

### Fibonacci heap

`insert` returns a `Node *` handle for `decrease_key`. A handle must
not be used after its node has been extracted and destroyed. Copying
the heap is disabled because it owns its nodes.

## Building and running

Compile any implementation as a standalone test programme:

```bash
clang++ -std=c++23 -O0 -ggdb3 \
  -Wall -Wextra -Wpedantic -Wconversion \
  -Wno-sign-conversion -Wshadow -Wswitch-enum \
  -Wimplicit-fallthrough -Wold-style-cast \
  -fno-omit-frame-pointer \
  -fsanitize=address,undefined,integer \
  -fno-sanitize=unsigned-integer-overflow \
  -fno-sanitize-recover=all -D_GLIBCXX_DEBUG \
  dsu.cpp -o dsu

./dsu
```

Replace `dsu.cpp` and `dsu` with the implementation you want to test.

## Purpose

These files are intended for study, experimentation, and use as a
readable reference. They keep the underlying mechanics visible rather
than hiding them behind heavy generic abstractions or premature
optimisation.
