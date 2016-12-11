#pragma once

#include "_Library.h"

#include "../Graphs/Graph.h"
#include <vector>
#include <map>
#include <memory>

namespace GraphClusterization
{
    using TClusterId = size_t;
    using TCluster = std::vector<Graphs::Graph::TVertex>;
    using TClusterMap = std::map<Graphs::Graph::TVertex, TClusterId>;

    class GRAPHCLUSTERIZATION_API Clusterization
    {
    public:
        Clusterization(const TClusterMap& i_cluster_map, const Graphs::Graph& i_graph);

        std::shared_ptr<Graphs::Graph> GetClusterGraph() const;

    private:
        std::shared_ptr<Graphs::Graph> _CreateClusterGraph();

    private:
        TClusterMap m_cluster_map;
        const Graphs::Graph& m_graph;
        std::shared_ptr<Graphs::Graph> mp_cluster_graph;
    };

}
