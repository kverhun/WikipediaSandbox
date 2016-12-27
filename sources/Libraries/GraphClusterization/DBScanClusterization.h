#pragma once

#include "_Library.h"
#include "Clusterization.h"

namespace GraphClusterization
{
    std::unique_ptr<Clusterization> GRAPHCLUSTERIZATION_API CreateGreedyClusterization(const Graphs::Graph& i_graph, size_t i_epsilon, size_t i_min_neighborhood_size);
}