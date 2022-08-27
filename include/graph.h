#pragma once
#include "node_ref.h"
#include <vector>

namespace star_map {

class Node;

class Graph {
public:
  Graph() = default;
  Graph(const Graph &);
  Graph &operator=(const Graph &);
  Graph(Graph &&) = default;
  Graph &operator=(Graph &&) = default;
  ~Graph();

  size_t Size() const { return nodes_.size(); }

  NodeRef AddNode();

  bool HasPath(size_t from, size_t to) const;
  bool HasPath(const NodeRef &from, const NodeRef &to) const {
    return HasPath(from.Id(), to.Id());
  }

  bool HasEdge(size_t from, size_t to) const;
  bool HasEdge(const NodeRef &from, const NodeRef &to) const {
    return HasEdge(from.Id(), to.Id());
  }

  bool AddEdge(size_t from, size_t to);
  bool AddEdge(const NodeRef &from, const NodeRef &to) {
    return AddEdge(from.Id(), to.Id());
  }

  bool RemoveEdge(size_t from, size_t to);
  bool RemoveEdge(const NodeRef &from, const NodeRef &to) {
    return RemoveEdge(from.Id(), to.Id());
  }

private:
  std::vector<Node *> nodes_;

  void Clear();
};

} // namespace star_map