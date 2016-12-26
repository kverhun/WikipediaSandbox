#include "Graph.h"

using namespace Graphs;

Graph::Graph(const Graph::TVertices& i_vertices, const Graph::TEdges& i_edges)
    : m_vertices(i_vertices)
    , m_edges(i_edges)
{}

const Graph::TVertices& Graph::GetVertices() const
{
    return m_vertices;
}

const Graph::TEdges& Graph::GetEdges() const
{
    return m_edges;
}

Graph::TEdges Graph::GetEdgesFromVertex(const Graph::TVertex& i_vertex) const
{
    if (m_edges_optimized.empty())
    {
        /*Graph::TEdges edges;
        for (const auto& edge : m_edges)
        {
            if (edge.first == i_vertex)
                edges.push_back(edge);
        }
        return edges;*/
        OptimizeForEdgesRetrieval();
    }
    return m_edges_optimized.at(i_vertex);
}

void Graph::OptimizeForEdgesRetrieval() const
{
    for (const auto& v : m_vertices)
        m_edges_optimized[v];
    for (const auto& e : m_edges)
        m_edges_optimized[e.first].push_back(e);
}