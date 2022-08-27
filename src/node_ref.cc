#include "node_ref.h"
#include "graph.h"

namespace star_map {

ChainExpr NodeRef::operator<=>(const NodeRef &other) const {
  return ChainExpr(*this) <=> other;
}

ChainExpr NodeRef::operator>(const NodeRef &other) const {
  return ChainExpr(*this) > other;
}

ChainExpr NodeRef::operator>(ChainExpr &&other) const {
  return ChainExpr(*this) > std::move(other);
}

ChainExpr NodeRef::operator<(const NodeRef &other) const {
  return ChainExpr(*this) < other;
}

ChainExpr NodeRef::operator<(ChainExpr &&other) const {
  return ChainExpr(*this) < std::move(other);
}

PathCheckExpr NodeRef::operator>>(const NodeRef &other) const {
  return PathCheckExpr(*this, other, ChainType::LTR);
}

PathCheckExpr NodeRef::operator<<(const NodeRef &other) const {
  return PathCheckExpr(*this, other, ChainType::RTL);
}

bool NodeRef::operator==(const NodeRef &other) const {
  return id_ == other.id_ && graph_ == other.graph_;
}

} // namespace star_map
