#pragma once

#include "_Library.h"

#include "Clusterization.h"

namespace GraphClusterization
{
    std::unique_ptr<Clusterization> GRAPHCLUSTERIZATION_API CreateRandomClusterization(const Graphs::Graph& i_graph, size_t i_number_of_clusters);
}