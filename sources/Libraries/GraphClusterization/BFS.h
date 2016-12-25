#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"

#include <set>

namespace Graphs
{
    Graph::TVertices GRAPHCLUSTERIZATION_API BfsGraphFromVertex(
        const Graph& i_graph,
        const Graph::TVertex& i_vertex_from
        );
}