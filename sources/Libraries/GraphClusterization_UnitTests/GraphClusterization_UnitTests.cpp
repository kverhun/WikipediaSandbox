#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../GraphClusterization/Clusterization.h"
#include "../GraphClusterization/RandomClusterization.h"

#include "../Graphs/Graph.h"

using namespace GraphClusterization;
using Graphs::Graph;

TEST_CASE("CreateClusterization")
{
    SECTION("TestClusterGraphCreation1")
    {
        Graph g{ {1,2}, {{1, 2}} };
        TClusterMap cluster_map{ {1,1}, {2,2} };

        Clusterization clusterization{ cluster_map, g };
        auto p_cluster_graph = clusterization.GetClusterGraph();

        const Graph::TVertices vertices_expected{ 1, 2 };
        const Graph::TEdges edges_expected{ {1, 2} };

        REQUIRE(nullptr != p_cluster_graph);
        REQUIRE(vertices_expected == p_cluster_graph->GetVertices());
        REQUIRE(edges_expected == p_cluster_graph->GetEdges());
	}

    SECTION("TestClusterGraphCreation2")
    {
        Graph g{ { 1,2,3,4 },{ { 1, 2 },{1,3},{3,2},{4,1},{4,3} } };
        TClusterMap cluster_map{ { 1,1 },{ 2,1 },{3,2},{4,2} };

        Clusterization clusterization{ cluster_map, g };
        auto p_cluster_graph = clusterization.GetClusterGraph();

        const Graph::TVertices vertices_expected{ 1, 2 };
        const Graph::TEdges edges_expected{ { 1, 2 }, {2, 1}};

        REQUIRE(nullptr != p_cluster_graph);
        REQUIRE(vertices_expected == p_cluster_graph->GetVertices());
        REQUIRE(edges_expected == p_cluster_graph->GetEdges());
    }

    SECTION("TestClusterGraphCreation3")
    {
        Graph g{ {1,2,3,4,5,6,7,8,9,10}, {{1,2}, {2,3}, {3,4}, {4,5}, {5,6}, {7,8}, {8,9}, {10,9}, {9,4}} };
        TClusterMap cluster_map{ {1,1},{2,1}, {3,1}, {4,2}, {5,2},{6,2}, {7,3}, {8,3}, {9,3}, {10,3} };

        Clusterization clusterization{ cluster_map, g };
        auto p_cluster_graph = clusterization.GetClusterGraph();
        const Graph::TVertices vertices_expected{1,2,3};
        const Graph::TEdges edges_expected{ {1,2},{3,2} };

        REQUIRE(nullptr != p_cluster_graph);
        REQUIRE(vertices_expected == p_cluster_graph->GetVertices());
        REQUIRE(edges_expected == p_cluster_graph->GetEdges());
    }
}

TEST_CASE("RandomClusterizationTests")
{
    SECTION("RandomClusterizationTest1")
    {
        Graph g{ { 1,2,3,4,5,6,7,8,9,10 },{ { 1,2 },{ 2,3 },{ 3,4 },{ 4,5 },{ 5,6 },{ 7,8 },{ 8,9 },{ 10,9 },{ 9,4 } } };

        auto p_clusterization = CreateRandomClusterization(g, 4);
        auto p_cluster_graph = p_clusterization->GetClusterGraph();
        
        REQUIRE(4 == p_cluster_graph->GetVertices().size());
    }

    SECTION("RandomClusterizationTest1")
    {
        Graph g{ { 1,2,3,4,5,6,7,8,9,10 },{ { 1,2 },{ 2,3 },{ 3,4 },{ 4,5 },{ 5,6 },{ 7,8 },{ 8,9 },{ 10,9 },{ 9,4 } } };

        auto p_clusterization = CreateRandomClusterization(g, 3);
        auto p_cluster_graph = p_clusterization->GetClusterGraph();

        REQUIRE(3 == p_cluster_graph->GetVertices().size());
    }
}