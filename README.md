# star-map

![test](https://github.com/KCFindstr/star-map/actions/workflows/test.yml/badge.svg)

Fun C++ graph library utilizing C++20 spaceship operator.

## Usage

```c++
#include "graph.h"

#include <gtest/gtest.h>

using star_map::Graph;

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
  // Remove the edge B -> D
  !(B > D);
  // Assert there is no path from A to E
  SM_ASSERT_FALSE(A >> E);
  // Assert there is a path from E to C
  SM_ASSERT_TRUE(C << E);
  // Assert there is no bidirectional edge between D and B
  SM_ASSERT_FALSE(D <=> B);
  // Assert the edge D -> B remains
  SM_ASSERT_TRUE(B < D);
  // Add the edge B -> D back
  B > D;
  // The operators are designed to be used in if-statements
  if (!(A >> E)) {
    FAIL() << "There should be a path from A to E";
  }
  if (A << E) {
    FAIL() << "There should be no path from E to A";
  }
  // You can also chain edge checks, which requires all edges to present
  if (!(A > B > D > E)) {
    FAIL() << "There should be a chain A -> B -> D -> E";
  }
}
```
