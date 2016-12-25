#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"

#include <set>
#include <limits>

namespace Graphs
{
    Graph::TVertices GRAPHCLUSTERIZATION_API BfsGraphFromVertex(
        const Graph& i_graph,
        const Graph::TVertex& i_vertex_from,
        size_t i_vertex_number_limit = std::numeric_limits<size_t>::max()
        );
}