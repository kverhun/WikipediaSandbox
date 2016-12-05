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

TEST_CASE("Test")
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