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
        auto traverse = Graphs::BfsGraphFromVertex(g, 1, [](size_t i_v_num, size_t) {return i_v_num >= 4; });
        REQUIRE(traverse == std::vector<Graphs::Graph::TVertex>({ 1, 4, 8, 5 }));
    }
    SECTION("Test3")
    {
        auto traverse = Graphs::BfsGraphFromVertex(g, 1, [](size_t, size_t i_depth) {return i_depth > 2; });
        REQUIRE(traverse == std::vector<Graphs::Graph::TVertex>({ 1, 4, 8, 5 }));
    }
}