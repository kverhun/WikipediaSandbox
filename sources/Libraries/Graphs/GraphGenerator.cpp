#include "GraphGenerator.h"
#include "Graph.h"

#include <random>

using namespace Graphs;

std::unique_ptr<Graph> Graphs::GenerateGraph(size_t i_vertex_number)
{
    Graph::TVertices vertices(i_vertex_number);
    for (size_t i = 0; i < i_vertex_number; ++i)
        vertices[i] = i;

    std::random_device rd;
    std::mt19937 gen(rd());

    Graph::TEdges edges;
    for (size_t i = 0; i < i_vertex_number; ++i)
    {
        std::uniform_int_distribution<> vertex_id_distribution(i, i_vertex_number-1);
        std::uniform_int_distribution<> number_of_connection_distribution(0, i_vertex_number-1);
        int vertex_number = number_of_connection_distribution(rd);
        for (int connection = 0; connection  < vertex_number; ++connection)
        {
            int vertex_to = vertex_id_distribution(rd);
            edges.emplace_back(i, vertex_to);
        }
    }

    return std::make_unique<Graph>(vertices, edges);
}