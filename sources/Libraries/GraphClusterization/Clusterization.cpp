#include "Clusterization.h"

using namespace GraphClusterization;
using Graphs::Graph;

Clusterization::Clusterization(const TClusterMap& i_cluster_map, const Graph& i_graph)
    : m_cluster_map(i_cluster_map)
    , m_graph(i_graph)
{
    mp_cluster_graph = _CreateClusterGraph();
}

std::shared_ptr<Graph> Clusterization::GetClusterGraph() const
{
    return mp_cluster_graph;
}

std::shared_ptr<Graph> Clusterization::_CreateClusterGraph()
{
    return nullptr;
}