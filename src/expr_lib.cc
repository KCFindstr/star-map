#include "expr_lib.h"
#include "common.h"
#include "graph.h"
#include "node_ref.h"
#include <iostream>

namespace star_map {

ChainExpr::ChainExpr(ChainExpr &&other)
    : chain_(std::move(other.chain_)), right_(other.right_),
      evaluated_(other.evaluated_), negated_(other.negated_),
      graph_(other.graph_) {
  LOG << "Addr: " << this << ",  ChainExpr::Moved from " << &other
      << ", E=" << evaluated_ << std::endl;
  other.evaluated_ = true;
}

ChainExpr::~ChainExpr() {
  LOG << "Addr: " << this << ", ChainExpr::~ChainExpr() E=" << evaluated_
      << ", N=" << negated_ << std::endl;
  if (!evaluated_) {
    evaluated_ = true;
    Execute();
  }
}

ChainExpr::operator bool() {
  LOG << "Addr: " << this << ", ChainExpr::operator bool()" << std::endl;
  if (!evaluated_) {
    evaluated_ = true;
    return Check() ^ negated_;
  }
  return false;
}

ChainExpr ChainExpr::operator<=>(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::Bi));
  right_ = other.Id();
  return std::move(*this);
}

ChainExpr ChainExpr::operator>(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::LTR));
  right_ = other.Id();
  return std::move(*this);
}

ChainExpr ChainExpr::operator>(ChainExpr &&other) {
  chain_.push_back(std::make_pair(right_, ChainType::LTR));
  right_ = other.right_;
  chain_.insert(chain_.end(), other.chain_.begin(), other.chain_.end());
  other.evaluated_ = true;
  return std::move(*this);
}

ChainExpr ChainExpr::operator<(const NodeRef &other) {
  chain_.push_back(std::make_pair(right_, ChainType::RTL));
  right_ = other.Id();
  return std::move(*this);
}

ChainExpr ChainExpr::operator<(ChainExpr &&other) {
  chain_.push_back(std::make_pair(right_, ChainType::RTL));
  right_ = other.right_;
  chain_.insert(chain_.end(), other.chain_.begin(), other.chain_.end());
  other.evaluated_ = true;
  return std::move(*this);
}

ChainExpr ChainExpr::operator!() {
  LOG << "Addr: " << this << ", ChainExpr::operator!()" << std::endl;
  negated_ = !negated_;
  return std::move(*this);
}

void ChainExpr::Execute() {
  for (size_t i = 0; i < chain_.size(); ++i) {
    size_t l = chain_[i].first;
    size_t r = i + 1 < chain_.size() ? chain_[i + 1].first : right_;
    switch (chain_[i].second) {
    case ChainType::Bi:
      LOG << "Execute Bi: " << l << " <=> " << r << std::endl;
      if (negated_) {
        graph_->RemoveEdge(l, r);
        graph_->RemoveEdge(r, l);
      } else {
        graph_->AddEdge(l, r);
        graph_->AddEdge(r, l);
      }
      break;
    case ChainType::LTR:
      LOG << "Execute LTR: " << l << " > " << r << std::endl;
      if (negated_) {
        graph_->RemoveEdge(l, r);
      } else {
        graph_->AddEdge(l, r);
      }
      break;
    case ChainType::RTL:
      LOG << "Execute RTL: " << l << " < " << r << std::endl;
      if (negated_) {
        graph_->RemoveEdge(r, l);
      } else {
        graph_->AddEdge(r, l);
      }
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

ChainExpr::ChainExpr(const NodeRef &node)
    : chain_(), right_(node.Id()), evaluated_(false), negated_(false),
      graph_(node.Graph()) {
  LOG << "Addr: " << this << ", ChainExpr::ChainExpr(const NodeRef &node)"
      << std::endl;
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