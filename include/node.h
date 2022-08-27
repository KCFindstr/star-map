#pragma once
#include <unordered_set>

namespace star_map {

class Graph;

class Node {
private:
  Node() = default;
  Node(const Node &) = default;
  Node &operator=(const Node &) = default;
  Node(Node &&) = delete;
  Node &operator=(Node &&) = delete;
  ~Node() = default;

  std::unordered_set<size_t> neighbors_;

  friend class Graph;
};

} // namespace star_map