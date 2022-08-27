#pragma once
#include <utility>
#include <vector>

namespace star_map {

class NodeRef;
class Graph;

enum class ChainType { None, Bi, LTR, RTL };

class ChainExpr {
public:
  ChainExpr() = delete;
  ChainExpr(const ChainExpr &) = delete;
  ChainExpr &operator=(const ChainExpr &) = delete;
  ChainExpr(ChainExpr &&);
  ChainExpr &operator=(ChainExpr &&) = delete;
  ~ChainExpr();
  operator bool();

  ChainExpr operator<=>(const NodeRef &other);
  ChainExpr operator>(const NodeRef &other);
  ChainExpr operator>(ChainExpr &&other);
  ChainExpr operator<(const NodeRef &other);
  ChainExpr operator<(ChainExpr &&other);
  ChainExpr operator!();

protected:
  ChainExpr(const NodeRef &node);

  virtual void Execute();
  virtual bool Check() const;

  std::vector<std::pair<int, ChainType>> chain_;
  size_t right_;
  bool evaluated_;
  bool negated_;
  Graph *graph_;

  friend class NodeRef;
};

class PathCheckExpr {
public:
  PathCheckExpr() = delete;
  PathCheckExpr(const PathCheckExpr &) = delete;
  PathCheckExpr &operator=(const PathCheckExpr &) = delete;
  PathCheckExpr(PathCheckExpr &&) = delete;
  PathCheckExpr &operator=(PathCheckExpr &&) = delete;
  ~PathCheckExpr() = default;

  operator bool() const;

protected:
  PathCheckExpr(const NodeRef &left, const NodeRef &right, ChainType type);
  size_t left_;
  size_t right_;
  ChainType type_;
  Graph *graph_;

  friend class NodeRef;
};

} // namespace star_map