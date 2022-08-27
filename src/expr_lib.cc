#include "expr_lib.h"
#include "graph.h"
#include "node_ref.h"

namespace star_map {

ChainExpr::~ChainExpr() {
  if (!evaluated_) {
    evaluated_ = true;
    Execute();
  }
}

ChainExpr::operator bool() {
  evaluated_ = true;
  return Check();
}

ChainExpr &ChainExpr::operator<=>(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::Bi));
  right_ = other.Id();
  return *this;
}

ChainExpr &ChainExpr::operator>(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::LTR));
  right_ = other.Id();
  return *this;
}

ChainExpr &ChainExpr::operator>(const ChainExpr &other) {
  chain_.push_back(std::make_pair(right_, ChainType::LTR));
  right_ = other.right_;
  chain_.insert(chain_.end(), other.chain_.begin(), other.chain_.end());
  return *this;
}

ChainExpr &ChainExpr::operator<(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::RTL));
  right_ = other.Id();
  return *this;
}

ChainExpr &ChainExpr::operator<(const ChainExpr &other) {
  chain_.push_back(std::make_pair(right_, ChainType::RTL));
  right_ = other.right_;
  chain_.insert(chain_.end(), other.chain_.begin(), other.chain_.end());
  return *this;
}

void ChainExpr::Execute() {
  for (size_t i = 0; i < chain_.size(); ++i) {
    size_t l = chain_[i].first;
    size_t r = i + 1 < chain_.size() ? chain_[i + 1].first : right_;
    switch (chain_[i].second) {
    case ChainType::Bi:
      graph_->AddEdge(l, r);
      graph_->AddEdge(r, l);
      break;
    case ChainType::LTR:
      graph_->AddEdge(l, r);
      break;
    case ChainType::RTL:
      graph_->AddEdge(r, l);
      break;
    default:
      break;
    }
  }
}

bool ChainExpr::Check() const {
  for (size_t i = 0; i < chain_.size(); ++i) {
    size_t l = chain_[i].first;
    size_t r = i + 1 < chain_.size() ? chain_[i + 1].first : right_;
    switch (chain_[i].second) {
    case ChainType::Bi:
      if (!graph_->HasEdge(l, r) || !graph_->HasEdge(r, l))
        return false;
      break;
    case ChainType::LTR:
      if (!graph_->HasEdge(l, r))
        return false;
      break;
    case ChainType::RTL:
      if (!graph_->HasEdge(r, l))
        return false;
      break;
    default:
      break;
    }
  }
  return true;
}

ChainExpr::ChainExpr(const NodeRef &left, const NodeRef &right,
                     ChainType chainType)
    : chain_(), right_(right.Id()), evaluated_(false), graph_(right.Graph()) {
  chain_.push_back(std::make_pair(left.Id(), chainType));
}

PathCheckExpr::operator bool() const {
  switch (type_) {
  case ChainType::LTR:
    return graph_->HasPath(left_, right_);
  case ChainType::RTL:
    return graph_->HasPath(right_, left_);
  default:
    return false;
  }
}

PathCheckExpr::PathCheckExpr(const NodeRef &left, const NodeRef &right,
                             ChainType type)
    : left_(left.Id()), right_(right.Id()), type_(type), graph_(left.Graph()) {}

} // namespace star_map