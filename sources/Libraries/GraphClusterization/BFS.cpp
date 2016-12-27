#include "BFS.h"

#include <queue>

namespace Graphs
{
    Graph::TVertices BfsGraphFromVertex(const Graph& i_graph, const Graph::TVertex& i_vertex_from, BFS::TSearchStopCondition i_stop_condition)
    {
        Graph::TVertices traverse;
        std::set<Graph::TVertex> visited_during_current_traverse;
        std::queue<Graph::TVertex> q;
        q.push(i_vertex_from);

        std::map<Graph::TVertex, size_t> depth_map;
        depth_map[i_vertex_from] = 0;

        while (!q.empty())
        {
            auto current_vertex = q.front();
            q.pop();

            size_t current_depth = depth_map[current_vertex];

            if (i_stop_condition && i_stop_condition(traverse.size(), current_depth))
                return traverse;

            traverse.push_back(current_vertex);
            visited_during_current_traverse.insert(current_vertex);
            auto edges_from_current_vertex = i_graph.GetEdgesFromVertex(current_vertex);
            for (const auto& e : edges_from_current_vertex)
            {
                if (visited_during_current_traverse.find(e.second) == visited_during_current_traverse.end())
                {
                    q.push(e.second);
                    visited_during_current_traverse.insert(e.second);
                    depth_map[e.second] = current_depth + 1;
                }
            }
        }
    
        return traverse;
    }
}