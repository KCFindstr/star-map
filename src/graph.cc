#include "graph.h"
#include "node.h"
#include <queue>

using NodeQueue = std::queue<size_t>;

namespace star_map {

Graph::Graph(const Graph &graph) { *this = graph; }

Graph &Graph::operator=(const Graph &graph) {
  if (this != &graph) {
    Clear();
    for (auto node : graph.nodes_) {
      nodes_.push_back(new Node(*node));
    }
  }
  return *this;
}

Graph::~Graph() { Clear(); }

NodeRef Graph::AddNode() {
  nodes_.push_back(new Node);
  return NodeRef(this, nodes_.size() - 1);
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
    for (auto neighbor : nodes_[node]->neighbors_) {
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
  return nodes_[from]->neighbors_.find(to) != nodes_[from]->neighbors_.end();
}

bool Graph::AddEdge(size_t from, size_t to) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  if (HasEdge(from, to)) {
    return false;
  }
  nodes_[from]->neighbors_.insert(to);
  return true;
}

bool Graph::RemoveEdge(size_t from, size_t to) {
  if (from >= nodes_.size() || to >= nodes_.size()) {
    return false;
  }
  if (!HasEdge(from, to)) {
    return false;
  }
  nodes_[from]->neighbors_.erase(to);
  return true;
}

void Graph::Clear() {
  for (auto node : nodes_) {
    delete node;
  }
  nodes_.clear();
}

} // namespace star_map