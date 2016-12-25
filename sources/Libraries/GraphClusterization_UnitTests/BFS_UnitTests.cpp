#include <catch.hpp>

#include "../GraphClusterization/BFS.h"
#include "../Graphs/Graph.h"

#include <iostream>

TEST_CASE("BFS1")
{
    Graphs::Graph g({ 1,2,3,4,5,6,7,8,9 }, { {1,4}, {2,3}, {3,6}, {3,7}, {4,8},{4,5},{5,2},{5,6},{7,6} });
    SECTION("Test1")
    {
        auto traverse = Graphs::BfsGraphFromVertex(g, 1);
        REQUIRE(traverse == std::vector<Graphs::Graph::TVertex>({ 1, 4, 8, 5, 2, 6, 3, 7 }));
    }
    SECTION("Test2")
    {
        auto traverse = Graphs::BfsGraphFromVertex(g, 1, 4);
        REQUIRE(traverse == std::vector<Graphs::Graph::TVertex>({ 1, 4, 8, 5 }));
    }
}