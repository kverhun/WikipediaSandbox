#pragma once

#include "_Library.h"

#include <vector>

namespace Graphs
{
    class GRAPHS_API Graph
    {
    public:
        using TVertex   = size_t;
        using TEdge     = std::pair<TVertex, TVertex>;
        using TVertices = std::vector<TVertex>;
        using TEdges    = std::vector<TEdge>;
    public:
        Graph(const std::vector<TVertex>& i_vertices, const std::vector<TEdge>& i_edges);

        const TVertices& GetVertices() const;
        const TEdges&    GetEdges()    const;

        TEdges           GetEdgesFromVertex(const TVertex& i_vertex) const;
    private:
        TVertices m_vertices;
        TEdges    m_edges;
    };
}