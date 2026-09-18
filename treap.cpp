// vicfred
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

struct Treap {
private:
  struct Node {
    int64_t key;
    uint64_t priority;
    Node *left;
    Node *right;

    Node(int64_t initial_key,
         uint64_t initial_priority)
        : key(initial_key),
          priority(initial_priority),
          left(nullptr),
          right(nullptr) {
    }
  };

  Node *root;
  mt19937_64 rng;

  // Split tree into:
  // left:  keys < key
  // right: keys >= key
  // Time: expected O(log n)
  // Space: expected O(log n) recursion stack
  void split(Node *node, int64_t key,
             Node *&left, Node *&right) {
    if (node == nullptr) {
      left = nullptr;
      right = nullptr;
      return;
    }

    if (node->key < key) {
      split(node->right, key,
            node->right, right);
      left = node;
    } else {
      split(node->left, key,
            left, node->left);
      right = node;
    }
  }

  // Merge two trees where every key in left
  // is smaller than every key in right.
  // Time: expected O(log n)
  // Space: expected O(log n) recursion stack
  Node *merge(Node *left, Node *right) {
    if (left == nullptr) {
      return right;
    }

    if (right == nullptr) {
      return left;
    }

    if (left->priority > right->priority) {
      left->right =
          merge(left->right, right);
      return left;
    }

    right->left =
        merge(left, right->left);
    return right;
  }

  // Insert node while preserving both:
  // - BST order by key
  // - heap order by priority
  // Time: expected O(log n)
  // Space: expected O(log n) recursion stack
  Node *insert(Node *subtree, Node *node) {
    if (subtree == nullptr) {
      return node;
    }

    if (node->priority > subtree->priority) {
      split(subtree, node->key,
            node->left, node->right);
      return node;
    }

    if (node->key < subtree->key) {
      subtree->left =
          insert(subtree->left, node);
    } else {
      subtree->right =
          insert(subtree->right, node);
    }

    return subtree;
  }

  // Remove key from the tree.
  // Time: expected O(log n)
  // Space: expected O(log n) recursion stack
  Node *erase(Node *subtree, int64_t key) {
    if (subtree == nullptr) {
      return nullptr;
    }

    if (key == subtree->key) {
      Node *result =
          merge(subtree->left, subtree->right);

      delete subtree;
      return result;
    }

    if (key < subtree->key) {
      subtree->left =
          erase(subtree->left, key);
    } else {
      subtree->right =
          erase(subtree->right, key);
    }

    return subtree;
  }

  // Search for a key.
  // Time: expected O(log n)
  // Space: O(1)
  bool contains(Node *node, int64_t key) const {
    while (node != nullptr) {
      if (key == node->key) {
        return true;
      }

      if (key < node->key) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    return false;
  }

  // Store the keys in sorted order.
  // Time: O(n)
  // Space: expected O(log n) recursion stack
  void inorder(Node *node,
               vector<int64_t> &values) const {
    if (node == nullptr) {
      return;
    }

    inorder(node->left, values);
    values.push_back(node->key);
    inorder(node->right, values);
  }

  // Delete every node.
  // Time: O(n)
  // Space: expected O(log n) recursion stack
  void destroy(Node *node) {
    if (node == nullptr) {
      return;
    }

    destroy(node->left);
    destroy(node->right);

    delete node;
  }

public:
  Treap()
      : root(nullptr),
        rng(random_device{}()) {
  }

  Treap(const Treap &) = delete;

  Treap &operator=(const Treap &) = delete;

  ~Treap() {
    destroy(root);
  }

  // Insert key if it is not already present.
  // Time: expected O(log n)
  void insert(int64_t key) {
    if (contains(key)) {
      return;
    }

    Node *node = new Node(key, rng());
    root = insert(root, node);
  }

  // Remove key if it exists.
  // Time: expected O(log n)
  void erase(int64_t key) {
    root = erase(root, key);
  }

  // Check whether key exists.
  // Time: expected O(log n)
  bool contains(int64_t key) const {
    return contains(root, key);
  }

  // Return all keys in sorted order.
  // Time: O(n)
  vector<int64_t> values() const {
    vector<int64_t> result;
    inorder(root, result);
    return result;
  }
};

int main() {
  {
    Treap treap;

    treap.insert(5);
    treap.insert(2);
    treap.insert(8);
    treap.insert(1);
    treap.insert(4);

    assert(treap.contains(1));
    assert(treap.contains(2));
    assert(treap.contains(4));
    assert(treap.contains(5));
    assert(treap.contains(8));

    assert(!treap.contains(3));
    assert(!treap.contains(10));

    vector<int64_t> expected = {
        1, 2, 4, 5, 8
    };

    assert(treap.values() == expected);
  }

  {
    Treap treap;

    treap.insert(10);
    treap.insert(5);
    treap.insert(15);
    treap.insert(3);
    treap.insert(7);

    treap.erase(5);

    assert(!treap.contains(5));
    assert(treap.contains(3));
    assert(treap.contains(7));

    vector<int64_t> expected = {
        3, 7, 10, 15
    };

    assert(treap.values() == expected);

    treap.erase(10);

    expected = {
        3, 7, 15
    };

    assert(treap.values() == expected);
  }

  {
    Treap treap;

    treap.insert(42);
    treap.insert(42);
    treap.insert(42);

    vector<int64_t> expected = {42};

    assert(treap.values() == expected);

    treap.erase(42);

    assert(!treap.contains(42));
    assert(treap.values().empty());
  }

  {
    Treap treap;

    for (int64_t i = 1; i <= 100; ++i) {
      treap.insert(i);
    }

    for (int64_t i = 1; i <= 100; ++i) {
      assert(treap.contains(i));
    }

    for (int64_t i = 2; i <= 100; i += 2) {
      treap.erase(i);
    }

    for (int64_t i = 1; i <= 100; ++i) {
      if (i % 2 == 0) {
        assert(!treap.contains(i));
      } else {
        assert(treap.contains(i));
      }
    }
  }

  cout << "All tests passed." << endl;

  return 0;
}
