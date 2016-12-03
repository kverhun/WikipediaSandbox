#pragma once

#include "_Library.h"

#include <memory>

namespace Graphs
{
    class Graph;

    std::unique_ptr<Graph> GRAPHS_API GenerateGraph(size_t i_vertex_number);
}