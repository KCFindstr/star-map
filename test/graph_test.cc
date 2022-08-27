#include "graph.h"

#include <gtest/gtest.h>
#include <string>

namespace star_map {

#define SM_ASSERT_TRUE(expr) ASSERT_TRUE(bool(expr))
#define SM_ASSERT_FALSE(expr) ASSERT_FALSE(bool(expr))

TEST(GraphTest, ExampleTest) {
  // Create a graph
  Graph graph;
  // Create 5 nodes
  auto [A, B, C, D, E] = graph.Nodes<5>();
  // Creates a chain of nodes A -> B -> C
  A > B > C;
  // Creates a bidirectional chain of nodes B <-> D <-> E
  B <=> D <=> E;
  // Assert there is an edge from A to B
  SM_ASSERT_TRUE(A > B);
  // Assert there is no edge from B to A
  SM_ASSERT_FALSE(A < B);
  // Assert there is a bidirectional edge between D and B
  SM_ASSERT_TRUE(D <=> B);
  // Assert there is no bidirectional edge between A and B
  SM_ASSERT_FALSE(A <=> B);
  // Assert there is a path from A to E (A -> B -> D -> E)
  SM_ASSERT_TRUE(A >> E);
  // Assert there is no path from E to A
  SM_ASSERT_FALSE(E >> A);
  // Assert there is a path from E to C (E -> D -> B -> C)
  SM_ASSERT_TRUE(C << E);
  // Remove the edge D -> E
  !(D > E);
  // Assert there is no path from A to E
  SM_ASSERT_FALSE(A >> E);
  // Assert there is a path from E to C
  SM_ASSERT_TRUE(C << E);
  // Add the edge D -> E back
  D > E;
  // Assert there is a path from A to E
  SM_ASSERT_TRUE(A >> E);
}

TEST(GraphTest, PathChain) {
  Graph graph;
  auto A = graph.Node();
  auto B = graph.Node();
  auto C = graph.Node();
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
  SM_ASSERT_FALSE(A > A);
  SM_ASSERT_FALSE(A < A);
  SM_ASSERT_FALSE(B >> A)
      << "Unexpected modification to graph allowing path from B to A";
}

TEST(GraphTest, SpaceShipReady) {
  Graph graph;
  auto A = graph.Node();
  auto B = graph.Node();
  A <=> B;
  // Path
  ASSERT_TRUE(A >> A);
  ASSERT_TRUE(B << A);
  ASSERT_TRUE(A >> B);

  // Edge
  SM_ASSERT_TRUE(A > B);
  SM_ASSERT_TRUE(B > A);
  SM_ASSERT_TRUE(A <=> B);
}

TEST(GraphTest, SpaceShipChain1) {
  Graph graph;
  auto A = graph.Node();
  auto B = graph.Node();
  auto C = graph.Node();
  A <=> B <=> C;
  // Path
  ASSERT_TRUE(A >> A);
  ASSERT_TRUE(C << A);
  ASSERT_TRUE(A >> C);

  // Edge
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

TEST(GraphTest, SpaceShipChainMixed) {
  Graph graph;
  auto [A, B, C, D] = graph.Nodes<4>();
  D > A <=> B < C;
  // Path
  ASSERT_TRUE(A >> A);
  ASSERT_TRUE(C >> A);
  ASSERT_FALSE(A >> C);
  ASSERT_FALSE(A >> D);

  // Edge
  SM_ASSERT_TRUE(D > A);
  SM_ASSERT_TRUE(C > B);
  SM_ASSERT_TRUE(A <=> B);
  SM_ASSERT_FALSE(A > C);
  SM_ASSERT_FALSE(C > A);
  SM_ASSERT_FALSE(A > D);
  SM_ASSERT_FALSE(A <=> D);
}

TEST(GraphTest, ChainCheck) {
  Graph graph;
  auto [A, B, C, D] = graph.Nodes<4>();
  D > A <=> B < C;
  // clang-format off
  SM_ASSERT_TRUE(D > A > B < C);
  SM_ASSERT_TRUE(B <=> A < D);
  SM_ASSERT_TRUE(A < B < C > B <=> A < D);

  SM_ASSERT_FALSE(D > B > C);
  SM_ASSERT_FALSE(D > A > B > C);
  SM_ASSERT_FALSE(D > A > B < C < D);
  // clang-format on
}

TEST(GraphTest, TreeCheck) {
  Graph graph;
  int N = 16;
  auto nodes = graph.Nodes(N);
  for (int i = 1; i < N; ++i) {
    nodes[i] < nodes[i >> 1];
  }
  for (int i = 1; i < N; ++i) {
    SM_ASSERT_TRUE(nodes[0] >> nodes[i]) << "Root cannot reach " << i;
    SM_ASSERT_FALSE(nodes[0] << nodes[i])
        << i << " should not be able to reach root";
  }
}

TEST(GraphTest, LoopCheck) {
  Graph graph;
  int N = 16;
  auto nodes = graph.Nodes(N);
  for (int i = 0; i < N; ++i) {
    nodes[i] > nodes[(i + 1) % N];
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      SM_ASSERT_TRUE(nodes[i] >> nodes[j]) << i << " cannot reach " << j;
    }
  }
}

TEST(GraphTest, Negate) {
  Graph graph;
  auto [A, B, C, D] = graph.Nodes<4>();
  A <=> B > C > D;
  SM_ASSERT_TRUE(A >> D);
  SM_ASSERT_TRUE(A <=> B);
  SM_ASSERT_FALSE(!(A > B > C));
  SM_ASSERT_TRUE(!(A > B > C > A));
  // Remove edge
  !(A > B > C > D);
  SM_ASSERT_FALSE(A >> D);
  SM_ASSERT_FALSE(A <=> B);
  SM_ASSERT_FALSE(A > B);
  SM_ASSERT_FALSE(B > C);
  SM_ASSERT_FALSE(C > D);
  SM_ASSERT_TRUE(B > A);
  // Add back
  A > B;
  SM_ASSERT_TRUE(A <=> B);
}

} // namespace star_map