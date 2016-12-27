#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"

#include <set>
#include <limits>
#include <functional>

namespace Graphs
{
    namespace BFS
    {
        using TSearchStopCondition = std::function<bool(size_t i_vertices_found, size_t i_current_depth)>;
    }

    Graph::TVertices GRAPHCLUSTERIZATION_API BfsGraphFromVertex(
        const Graph& i_graph,
        const Graph::TVertex& i_vertex_from,
        BFS::TSearchStopCondition i_stop_condition = nullptr
        );
}