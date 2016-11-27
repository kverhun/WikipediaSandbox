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