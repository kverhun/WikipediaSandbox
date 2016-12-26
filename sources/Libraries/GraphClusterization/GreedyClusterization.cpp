#include "GreedyClusterization.h"

#include "BFS.h"

#include <iostream>
#include <set>

using namespace GraphClusterization;

std::unique_ptr<Clusterization> GraphClusterization::CreateGreedyClusterization(const Graphs::Graph& i_graph, size_t i_number_of_clusters)
{
    static size_t cnt = 0;

    i_graph.OptimizeForEdgesRetrieval();

    TClusterMap cluster_map;

    const size_t cluster_size = i_graph.GetVertices().size() / i_number_of_clusters;

    std::set<Graphs::Graph::TVertex> vertices_remained(i_graph.GetVertices().begin(), i_graph.GetVertices().end());
    std::set<Graphs::Graph::TVertex> vertices_marked;
    
    size_t current_cluster_idx = 1;
    size_t current_cluster_size = 0;
    while (!vertices_remained.empty())
    {
        ++cnt;
        if (cnt == 100)
        {
            cnt = 0;
            std::cout << "Processed: " << vertices_marked.size() << " of " << i_graph.GetVertices().size() << std::endl;
        }

        auto current_vertex = *vertices_remained.begin();
        vertices_remained.erase(current_vertex);
        auto bfs_from_current_vertex = Graphs::BfsGraphFromVertex(i_graph, current_vertex, cluster_size);
        for (const auto& v : bfs_from_current_vertex)
        {
            if (vertices_marked.find(v) == vertices_marked.end())
            {
                cluster_map[v] = current_cluster_idx;
                ++current_cluster_size;
                vertices_marked.insert(v);
                vertices_remained.erase(v);
            }
        }
        
        if (current_cluster_size >= 0.75 * cluster_size)
        {
            ++current_cluster_idx;
            current_cluster_size = 0;
        }
    }

    std::map<TClusterId, Graphs::Graph::TVertices> clusters;
    for (const auto& e : cluster_map)
    {
        clusters[e.second].push_back(e.first);
    }
    //for (const auto& c : clusters)
    //{
    //    std::cout << "Cluster: " << c.first << "; size: " << c.second.size() << std::endl;
    //}

    return std::make_unique<Clusterization>(cluster_map, i_graph);
}