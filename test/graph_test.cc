#include "graph.h"

#include <gtest/gtest.h>
#include <string>

namespace star_map {

#define SM_ASSERT_TRUE(expr) ASSERT_TRUE(bool(expr))
#define SM_ASSERT_FALSE(expr) ASSERT_FALSE(bool(expr))

TEST(GraphTest, PathChain) {
  Graph graph;
  auto A = graph.AddNode();
  auto B = graph.AddNode();
  auto C = graph.AddNode();
  A > B > C;
  // Path
  ASSERT_TRUE(A >> A);
  ASSERT_TRUE(B << A);
  ASSERT_TRUE(A >> C);
  ASSERT_FALSE(A << C);
  ASSERT_FALSE(C >> B);

  // Edge
  SM_ASSERT_TRUE(A > B);
  SM_ASSERT_FALSE(B > A);
  SM_ASSERT_FALSE(A > C);
  SM_ASSERT_FALSE(B >> A)
      << "Unexpected modification to graph allowing path from B to A";
}

TEST(GraphTest, SpaceShipReady) {
  Graph graph;
  auto A = graph.AddNode();
  auto B = graph.AddNode();
  A <=> B;
  SM_ASSERT_TRUE(A > B);
  SM_ASSERT_TRUE(B > A);
  SM_ASSERT_TRUE(A <=> B);
}

TEST(GraphTest, SpaceShipChain1) {
  Graph graph;
  auto A = graph.AddNode();
  auto B = graph.AddNode();
  auto C = graph.AddNode();
  A <=> B <=> C;
  SM_ASSERT_TRUE(A > B);
  SM_ASSERT_TRUE(B > A);
  SM_ASSERT_TRUE(A <=> B);
  SM_ASSERT_TRUE(B > C);
  SM_ASSERT_TRUE(C > B);
  SM_ASSERT_TRUE(B <=> C);
  SM_ASSERT_FALSE(A > C);
  SM_ASSERT_FALSE(C > A);
  SM_ASSERT_FALSE(A <=> C);
}

} // namespace star_map