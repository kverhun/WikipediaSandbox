#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <algorithm>
#include <sstream>

#include "../GraphsIO/GraphIO.h"

using namespace Graphs;

namespace
{
    void _RequireVertexPresence(const Graph::TVertex& i_vertex, const Graph::TVertices& i_vertices)
    {
        REQUIRE(std::find(i_vertices.begin(), i_vertices.end(), i_vertex) != i_vertices.end());
    }
    void _RequireEdgePresence(const Graph::TEdge& i_edge, const Graph::TEdges& i_edges)
    {
        REQUIRE(std::find(i_edges.begin(), i_edges.end(), i_edge) != i_edges.end());
    }
    void _RequireEdgeAbsence(const Graph::TEdge& i_edge, const Graph::TEdges& i_edges)
    {
        REQUIRE(std::find(i_edges.begin(), i_edges.end(), i_edge) == i_edges.end());
    }
}

TEST_CASE("TestGraphRead")
{
    SECTION("TestReadGraph_1")
    {
        std::istringstream stream(
            R"(1,2,4"
               2,3,4
               3
               4)");
        auto p_graph = GraphsIO::ReadGraphFromStream(stream);

        auto graph_vertexes = p_graph->GetVertices();
        auto graph_edges = p_graph->GetEdges();

        REQUIRE(graph_vertexes.size() == 4);
        REQUIRE(graph_edges.size() == 4);

        _RequireVertexPresence(1, graph_vertexes);
        _RequireVertexPresence(2, graph_vertexes);
        _RequireVertexPresence(3, graph_vertexes);
        _RequireVertexPresence(4, graph_vertexes);

        _RequireEdgePresence({ 1, 2 }, graph_edges);
        _RequireEdgePresence({ 1, 4 }, graph_edges);
        _RequireEdgePresence({ 2, 3 }, graph_edges);
        _RequireEdgePresence({ 2, 4 }, graph_edges);

        _RequireEdgeAbsence({ 3, 1 }, graph_edges);
        _RequireEdgeAbsence({ 3, 2 }, graph_edges);
        _RequireEdgeAbsence({ 4, 2 }, graph_edges);
        _RequireEdgeAbsence({ 4, 3 }, graph_edges);
    }
    SECTION("TestReadGraph2")
    {
        std::istringstream stream(
            R"(1,2,4
            2,3
            3,5
            4,3,5
            5)");
        auto p_graph = GraphsIO::ReadGraphFromStream(stream);

        auto graph_vertexes = p_graph->GetVertices();
        auto graph_edges = p_graph->GetEdges();

        REQUIRE(graph_vertexes.size() == 5);
        REQUIRE(graph_edges.size() == 6);

        _RequireVertexPresence(1, graph_vertexes);
        _RequireVertexPresence(2, graph_vertexes);
        _RequireVertexPresence(3, graph_vertexes);
        _RequireVertexPresence(4, graph_vertexes);
        _RequireVertexPresence(5, graph_vertexes);

        _RequireEdgePresence({ 1, 2 }, graph_edges);
        _RequireEdgePresence({ 1, 4 }, graph_edges);
        _RequireEdgePresence({ 2, 3 }, graph_edges);
        _RequireEdgePresence({ 4, 3 }, graph_edges);

    }
}

using Geometry::Point2d;

namespace
{
    void _RequireEqualTopology(const Graphs::TGraphTopology& i_expected, const Graphs::TGraphTopology& i_actual)
    {
        for (const auto& expected : i_expected)
            REQUIRE(std::find_if(i_actual.begin(), i_actual.end(), 
                [&](const std::pair<Graph::TVertex, Geometry::Point2d>& i_val) 
                {
                    return i_val.first == expected.first
                        && i_val.second.GetX() == expected.second.GetX()
                        && i_val.second.GetY() == expected.second.GetY();
                }) != i_actual.end());
        for (const auto& actual : i_actual)
            REQUIRE(std::find_if(i_expected.begin(), i_expected.end(), [&](const std::pair<Graph::TVertex, Geometry::Point2d>& i_val)
                {
                    return i_val.first == actual.first
                        && i_val.second.GetX() == actual.second.GetX()
                        && i_val.second.GetY() == actual.second.GetY();
                }) != i_expected.end());
    }
}

TEST_CASE("TestGraphTopologyRead")
{
    SECTION("TestGraphTopologyRead1")
    {
        std::istringstream stream(
            R"(1,1,1
            2,2,2
            3,2,1
            4,1,2)");

        auto graph_topology_actual = GraphsIO::ReadGraphTopologyFromStream(stream);
        Graphs::TGraphTopology graph_topology_expected = { {1, {1,1}}, {2,{2,2}}, {3, {2,1}}, {4, {1,2}} };
        _RequireEqualTopology(graph_topology_expected, graph_topology_actual);
    }
}