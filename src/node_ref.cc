#include "node_ref.h"
#include "graph.h"

namespace star_map {

ChainExpr NodeRef::operator<=>(const NodeRef &other) const {
  return ChainExpr(*this, other, ChainType::Bi);
}

ChainExpr NodeRef::operator>(const NodeRef &other) const {
  return ChainExpr(*this, other, ChainType::LTR);
}

ChainExpr NodeRef::operator<(const NodeRef &other) const {
  return ChainExpr(*this, other, ChainType::RTL);
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
