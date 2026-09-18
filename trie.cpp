// vicfred
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Trie {
private:
  struct Node {
    // children[c] is the node reached by
    // following character c.
    // -1 means that edge does not exist.
    array<int64_t, 26> children;

    // True if a complete word ends here.
    bool is_word;

    Node()
        : is_word(false) {
      children.fill(-1);
    }
  };

  // Node 0 is always the root.
  vector<Node> nodes;

public:
  // Create an empty trie.
  // Time: O(1)
  // Space: O(1)
  Trie()
      : nodes(1) {
  }

  // Insert word into the trie.
  // L = length of word.
  // Time: O(L)
  // Space: O(L) worst case
  void insert(const string &word) {
    int64_t node = 0;

    for (char c : word) {
      assert('a' <= c && c <= 'z');

      int64_t index =
          static_cast<int64_t>(c - 'a');

      if (nodes[node].children[index] == -1) {
        nodes[node].children[index] =
            static_cast<int64_t>(nodes.size());

        nodes.emplace_back();
      }

      node = nodes[node].children[index];
    }

    nodes[node].is_word = true;
  }

  // Check whether word is stored in the trie.
  // L = length of word.
  // Time: O(L)
  // Space: O(1)
  bool contains(const string &word) const {
    int64_t node = 0;

    for (char c : word) {
      assert('a' <= c && c <= 'z');

      int64_t index =
          static_cast<int64_t>(c - 'a');

      if (nodes[node].children[index] == -1) {
        return false;
      }

      node = nodes[node].children[index];
    }

    return nodes[node].is_word;
  }

  // Check whether some stored word starts
  // with prefix.
  // L = length of prefix.
  // Time: O(L)
  // Space: O(1)
  bool starts_with(const string &prefix) const {
    int64_t node = 0;

    for (char c : prefix) {
      assert('a' <= c && c <= 'z');

      int64_t index =
          static_cast<int64_t>(c - 'a');

      if (nodes[node].children[index] == -1) {
        return false;
      }

      node = nodes[node].children[index];
    }

    return true;
  }
};

int main() {
  {
    Trie trie;

    trie.insert("cat");
    trie.insert("car");
    trie.insert("dog");

    assert(trie.contains("cat"));
    assert(trie.contains("car"));
    assert(trie.contains("dog"));

    assert(!trie.contains("ca"));
    assert(!trie.contains("cow"));
    assert(!trie.contains("dogs"));

    assert(trie.starts_with("c"));
    assert(trie.starts_with("ca"));
    assert(trie.starts_with("cat"));
    assert(trie.starts_with("do"));

    assert(!trie.starts_with("co"));
    assert(!trie.starts_with("z"));
  }

  {
    Trie trie;

    trie.insert("a");
    trie.insert("ab");
    trie.insert("abc");

    assert(trie.contains("a"));
    assert(trie.contains("ab"));
    assert(trie.contains("abc"));

    assert(!trie.contains("abcd"));

    assert(trie.starts_with("a"));
    assert(trie.starts_with("ab"));
    assert(trie.starts_with("abc"));
  }

  {
    Trie trie;

    trie.insert("apple");

    assert(trie.contains("apple"));
    assert(!trie.contains("app"));

    assert(trie.starts_with("app"));

    trie.insert("app");

    assert(trie.contains("app"));
  }

  {
    Trie trie;

    assert(!trie.contains("hello"));
    assert(!trie.starts_with("hello"));

    trie.insert("hello");

    assert(trie.contains("hello"));
    assert(trie.starts_with("hell"));
  }

  cout << "All tests passed." << endl;

  return 0;
}
