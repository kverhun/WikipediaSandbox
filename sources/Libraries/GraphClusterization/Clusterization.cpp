#include "Clusterization.h"

#include <set>

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

const Graph& Clusterization::GetBaseGraph() const
{
    return m_graph;
}

std::shared_ptr<Graph> Clusterization::_CreateClusterGraph()
{
    std::set<Graph::TVertex> vertices;
    for (const auto& entry : m_cluster_map)
        vertices.insert(entry.second);

    std::map<TClusterId, Graph::TVertices> cluster_to_vertices;
    for (const auto& entry : m_cluster_map)
        cluster_to_vertices[entry.second].push_back(entry.first);

    std::set<Graph::TEdge> edges;
    for (const auto& e : m_graph.GetEdges())
    {
        auto cluster_from = m_cluster_map[e.first];
        auto cluster_to = m_cluster_map[e.second];
        if (cluster_from != cluster_to)
            edges.insert(std::make_pair(cluster_from, cluster_to));
    }

    return std::make_shared<Graph>(Graph::TVertices(vertices.begin(), vertices.end()), Graph::TEdges(edges.begin(), edges.end()));
}