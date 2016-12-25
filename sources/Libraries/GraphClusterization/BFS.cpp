#include "BFS.h"

#include <queue>

namespace Graphs
{
    Graph::TVertices BfsGraphFromVertex(const Graph& i_graph, const Graph::TVertex& i_vertex_from)
    {
        Graph::TVertices traverse;
        std::set<Graph::TVertex> visited_during_current_traverse;
        std::queue<Graph::TVertex> q;
        q.push(i_vertex_from);
        while (!q.empty())
        {
            auto current_vertex = q.front();
            q.pop();
            traverse.push_back(current_vertex);
            visited_during_current_traverse.insert(current_vertex);
            auto edges_from_current_vertex = i_graph.GetEdgesFromVertex(current_vertex);
            for (const auto& e : edges_from_current_vertex)
            {
                if (visited_during_current_traverse.find(e.second) == visited_during_current_traverse.end())
                {
                    q.push(e.second);
                    visited_during_current_traverse.insert(e.second);
                }
            }
        }
    
        return traverse;
    }
}