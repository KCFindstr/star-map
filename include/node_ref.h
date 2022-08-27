#pragma once
#include "expr_lib.h"

namespace star_map {

class Graph;

class NodeRef {
public:
  size_t Id() const { return id_; }
  star_map::Graph *Graph() const { return graph_; }

  ChainExpr operator<=>(const NodeRef &other) const;
  ChainExpr operator>(const NodeRef &other) const;
  ChainExpr operator>(ChainExpr &&other) const;
  ChainExpr operator<(const NodeRef &other) const;
  ChainExpr operator<(ChainExpr &&other) const;
  PathCheckExpr operator>>(const NodeRef &other) const;
  PathCheckExpr operator<<(const NodeRef &other) const;
  bool operator==(const NodeRef &other) const;

  NodeRef() = default;
  NodeRef(const NodeRef &) = default;
  NodeRef &operator=(const NodeRef &) = default;
  NodeRef(NodeRef &&) = default;
  NodeRef &operator=(NodeRef &&) = default;
  ~NodeRef() = default;

private:
  NodeRef(star_map::Graph *graph, size_t id) : graph_(graph), id_(id) {}
  star_map::Graph *graph_;
  size_t id_;

  friend class Graph;
};

} // namespace star_map