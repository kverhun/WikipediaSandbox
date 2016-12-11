#include "RandomClusterization.h"

#include <random>

using namespace GraphClusterization;

std::unique_ptr<Clusterization> GraphClusterization::CreateRandomClusterization(const Graphs::Graph& i_graph, size_t i_number_of_clusters)
{
    TClusterMap cluster_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> cluster_id_distribution(1, static_cast<int>(i_number_of_clusters));

    for (const auto& v : i_graph.GetVertices())
    {
        auto cluster = cluster_id_distribution(gen);
        cluster_map[v] = cluster;
    }

    return std::make_unique<Clusterization>(cluster_map, i_graph);
}