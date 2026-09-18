// vicfred
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct FibonacciHeap {
  struct Node {
    int64_t key;
    int64_t degree;
    bool mark;

    Node *parent;
    Node *child;

    // Nodes in a root list or child list form a
    // circular doubly linked list.
    Node *left;
    Node *right;

    explicit Node(int64_t initial_key)
        : key(initial_key),
          degree(0),
          mark(false),
          parent(nullptr),
          child(nullptr),
          left(this),
          right(this) {
    }
  };

private:
  Node *min_node;
  int64_t node_count;

  // Remove node from its circular linked list.
  // Time: O(1)
  void remove_from_list(Node *node) {
    node->left->right = node->right;
    node->right->left = node->left;

    node->left = node;
    node->right = node;
  }

  // Add node to the root list.
  // Time: O(1)
  void add_root(Node *node) {
    node->parent = nullptr;

    if (min_node == nullptr) {
      min_node = node;
      node->left = node;
      node->right = node;
      return;
    }

    node->left = min_node;
    node->right = min_node->right;

    min_node->right->left = node;
    min_node->right = node;
  }

  // Make y a child of x.
  // Assumes both nodes are currently isolated
  // from the root list.
  // Time: O(1)
  void link(Node *y, Node *x) {
    y->parent = x;
    y->mark = false;

    if (x->child == nullptr) {
      x->child = y;
      y->left = y;
      y->right = y;
    } else {
      Node *child = x->child;

      y->left = child;
      y->right = child->right;

      child->right->left = y;
      child->right = y;
    }

    ++x->degree;
  }

  // Combine roots having the same degree until
  // every root has a different degree.
  // Time: O(log n) amortised
  // Space: O(log n)
  void consolidate() {
    if (min_node == nullptr) {
      return;
    }

    // First collect all current roots.
    vector<Node *> roots;

    Node *current = min_node;

    do {
      roots.push_back(current);
      current = current->right;
    } while (current != min_node);

    // Isolate every root. This makes the linking
    // process easier to reason about.
    for (Node *root : roots) {
      root->left = root;
      root->right = root;
    }

    min_node = nullptr;

    // degree_table[d] contains the current root
    // having degree d.
    vector<Node *> degree_table;

    for (Node *x : roots) {
      int64_t degree = x->degree;

      while (true) {
        if (degree >=
            static_cast<int64_t>(
                degree_table.size())) {
          degree_table.resize(degree + 1,
                              nullptr);
        }

        if (degree_table[degree] == nullptr) {
          degree_table[degree] = x;
          break;
        }

        Node *y = degree_table[degree];

        // x should have the smaller key because
        // this is a min-heap.
        if (x->key > y->key) {
          swap(x, y);
        }

        degree_table[degree] = nullptr;

        link(y, x);

        degree = x->degree;
      }
    }

    // Rebuild the root list and find the new
    // minimum.
    for (Node *node : degree_table) {
      if (node == nullptr) {
        continue;
      }

      add_root(node);

      if (node->key < min_node->key) {
        min_node = node;
      }
    }
  }

  // Remove x from y's children and move x to
  // the root list.
  // Time: O(1)
  void cut(Node *x, Node *y) {
    if (x->right == x) {
      y->child = nullptr;
    } else {
      if (y->child == x) {
        y->child = x->right;
      }

      remove_from_list(x);
    }

    --y->degree;

    x->parent = nullptr;
    x->mark = false;

    add_root(x);
  }

  // Perform cascading cuts after a child has
  // been removed from a node.
  // Time: O(log n) amortised
  void cascading_cut(Node *node) {
    Node *parent = node->parent;

    if (parent == nullptr) {
      return;
    }

    // First lost child: remember it.
    if (!node->mark) {
      node->mark = true;
      return;
    }

    // Second lost child: cut this node too.
    cut(node, parent);
    cascading_cut(parent);
  }

  // Recursively destroy one circular list and
  // all child lists below it.
  // Time: O(n) over the entire heap
  void destroy_list(Node *start) {
    if (start == nullptr) {
      return;
    }

    vector<Node *> nodes;

    Node *current = start;

    do {
      nodes.push_back(current);
      current = current->right;
    } while (current != start);

    for (Node *node : nodes) {
      destroy_list(node->child);
      delete node;
    }
  }

public:
  FibonacciHeap()
      : min_node(nullptr),
        node_count(0) {
  }

  FibonacciHeap(const FibonacciHeap &) = delete;

  FibonacciHeap &operator=(
      const FibonacciHeap &) = delete;

  ~FibonacciHeap() {
    destroy_list(min_node);
  }

  // Number of elements.
  // Time: O(1)
  int64_t size() const {
    return node_count;
  }

  // Check whether the heap is empty.
  // Time: O(1)
  bool empty() const {
    return min_node == nullptr;
  }

  // Insert a value and return a handle to its
  // node.
  // Time: O(1) amortised
  Node *insert(int64_t key) {
    Node *node = new Node(key);

    add_root(node);

    if (node->key < min_node->key) {
      min_node = node;
    }

    ++node_count;

    return node;
  }

  // Return the minimum value.
  // Time: O(1)
  int64_t minimum() const {
    assert(min_node != nullptr);

    return min_node->key;
  }

  // Remove and return the minimum value.
  // Time: O(log n) amortised
  int64_t extract_min() {
    assert(min_node != nullptr);

    Node *minimum = min_node;
    int64_t result = minimum->key;

    // Save the children before modifying their
    // circular linked list.
    vector<Node *> children;

    if (minimum->child != nullptr) {
      Node *current = minimum->child;

      do {
        children.push_back(current);
        current = current->right;
      } while (current != minimum->child);
    }

    // Every child becomes a root.
    for (Node *child : children) {
      remove_from_list(child);

      child->parent = nullptr;
      child->mark = false;

      add_root(child);
    }

    if (minimum->right == minimum) {
      min_node = nullptr;
    } else {
      Node *next = minimum->right;

      remove_from_list(minimum);
      min_node = next;

      consolidate();
    }

    --node_count;

    delete minimum;

    return result;
  }

  // Decrease node's key.
  // new_key must not be greater than the
  // current key.
  // Time: O(1) amortised
  void decrease_key(Node *node,
                    int64_t new_key) {
    assert(node != nullptr);
    assert(new_key <= node->key);

    node->key = new_key;

    Node *parent = node->parent;

    if (parent != nullptr &&
        node->key < parent->key) {
      cut(node, parent);
      cascading_cut(parent);
    }

    if (node->key < min_node->key) {
      min_node = node;
    }
  }

  // Move every element from other into this
  // heap. other becomes empty.
  // Time: O(1)
  void meld(FibonacciHeap &other) {
    if (this == &other ||
        other.min_node == nullptr) {
      return;
    }

    if (min_node == nullptr) {
      min_node = other.min_node;
      node_count = other.node_count;

      other.min_node = nullptr;
      other.node_count = 0;

      return;
    }

    // Concatenate the two circular root lists.
    Node *a_right = min_node->right;
    Node *b_left = other.min_node->left;

    min_node->right = other.min_node;
    other.min_node->left = min_node;

    a_right->left = b_left;
    b_left->right = a_right;

    if (other.min_node->key < min_node->key) {
      min_node = other.min_node;
    }

    node_count += other.node_count;

    other.min_node = nullptr;
    other.node_count = 0;
  }
};

int main() {
  {
    FibonacciHeap heap;

    heap.insert(7);
    heap.insert(3);
    heap.insert(18);
    heap.insert(39);
    heap.insert(23);

    assert(heap.size() == 5);
    assert(heap.minimum() == 3);

    assert(heap.extract_min() == 3);
    assert(heap.minimum() == 7);

    assert(heap.extract_min() == 7);
    assert(heap.minimum() == 18);
  }

  {
    FibonacciHeap heap;

    auto *a = heap.insert(10);
    auto *b = heap.insert(20);
    auto *c = heap.insert(30);

    assert(heap.minimum() == 10);

    heap.decrease_key(c, 5);

    assert(heap.minimum() == 5);
    assert(heap.extract_min() == 5);

    heap.decrease_key(b, 1);

    assert(heap.minimum() == 1);
    assert(heap.extract_min() == 1);
    assert(heap.extract_min() == 10);

    (void)a;
  }

  {
    FibonacciHeap heap;

    for (int64_t i = 20; i >= 1; --i) {
      heap.insert(i);
    }

    for (int64_t i = 1; i <= 20; ++i) {
      assert(heap.minimum() == i);
      assert(heap.extract_min() == i);
    }

    assert(heap.empty());
  }

  {
    FibonacciHeap a;
    FibonacciHeap b;

    a.insert(10);
    a.insert(30);

    b.insert(5);
    b.insert(20);

    a.meld(b);

    assert(b.empty());
    assert(a.size() == 4);
    assert(a.minimum() == 5);

    assert(a.extract_min() == 5);
    assert(a.extract_min() == 10);
    assert(a.extract_min() == 20);
    assert(a.extract_min() == 30);
  }

  cout << "All tests passed." << endl;

  return 0;
}
