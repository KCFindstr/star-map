#include "graph.h"
#include "node.h"
#include <queue>

using NodeQueue = std::queue<size_t>;
using TNode = star_map::Node;

namespace star_map {

Graph::Graph(const Graph &other) { *this = other; }

Graph &Graph::operator=(const Graph &other) {
  if (this != &other) {
    Clear();
    for (auto node : other.nodes_) {
      nodes_.push_back(new TNode(*node));
    }
  }
  return *this;
}

Graph::Graph(Graph &&other) { *this = std::move(other); }

Graph &Graph::operator=(Graph &&other) {
  if (this != &other) {
    Clear();
    nodes_ = std::move(other.nodes_);
    other.nodes_.clear();
  }
  return *this;
}

Graph::~Graph() { Clear(); }

NodeRef Graph::Node() {
  nodes_.push_back(new TNode);
  return NodeRef(this, nodes_.size() - 1);
}

std::vector<NodeRef> Graph::Nodes(int count) {
  std::vector<NodeRef> result;
  result.reserve(count);
  for (int i = 0; i < count; ++i) {
    result.push_back(Node());
  }
  return result;
}

bool Graph::HasPath(size_t from, size_t to) const {
  if (from == to) {
    return true;
  }
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  NodeQueue queue;
  std::vector<bool> visited(nodes_.size(), false);
  queue.push(from);
  visited[from] = true;
  while (!queue.empty()) {
    size_t node = queue.front();
    queue.pop();
    for (auto neighbor : nodes_[node]->outgoing_) {
      if (neighbor == to) {
        return true;
      }
      if (!visited[neighbor]) {
        queue.push(neighbor);
        visited[neighbor] = true;
      }
    }
  }
  return false;
}

bool Graph::HasEdge(size_t from, size_t to) const {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  return nodes_[from]->outgoing_.find(to) != nodes_[from]->outgoing_.end();
}

bool Graph::AddEdge(size_t from, size_t to) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  if (HasEdge(from, to)) {
    return false;
  }
  nodes_[from]->outgoing_.insert(to);
  return true;
}

bool Graph::RemoveEdge(size_t from, size_t to) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  if (!HasEdge(from, to)) {
    return false;
  }
  nodes_[from]->outgoing_.erase(to);
  return true;
}

void Graph::Clear() {
  for (auto node : nodes_) {
    delete node;
  }
  nodes_.clear();
}

} // namespace star_map