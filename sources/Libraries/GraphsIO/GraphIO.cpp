#include "GraphIO.h"

#include "../Geometry/Point2d.h"

#include <string>
#include <sstream>

using namespace GraphsIO;
using Graphs::Graph;

namespace
{
    const std::string g_id_regex = "([0-9]+,)+([0-9]+)";

    std::vector<std::string> _Tokenize(const std::string& i_line, const char i_delimiter = ',')
    {
        std::vector<std::string> res;

        std::stringstream ss(i_line);
        std::string item;
        while (std::getline(ss, item, ','))
            res.push_back(item);

        return res;
    }

    std::vector<Graph::TVertex> _ReadGraphLine(const std::string& i_line)
    {
        std::vector<Graph::TVertex> res;

        auto tokens = _Tokenize(i_line);
        for (const auto& t : tokens)
            res.push_back(std::stoi(t));

        return res;
    }

    std::pair<Graph::TVertex, Geometry::Point2d> _ReadTopologyLine(const std::string& i_line)
    {
        auto tokens = _Tokenize(i_line);
        Graph::TVertex vertex = std::stoi(tokens[0]);
        Geometry::Point2d point{static_cast<double>(std::stoi(tokens[1])), static_cast<double>(std::stoi(tokens[2]))};
        return std::make_pair(vertex, point);
    }

}

std::unique_ptr<Graphs::Graph> GraphsIO::ReadGraphFromStream(std::istream& i_stream)
{
    Graph::TVertices vertices;
    Graph::TEdges edges;

    std::string line;
    while (!i_stream.eof())
    {
        std::getline(i_stream, line);
        if (line.empty())
            continue;

        auto vertices_in_line = _ReadGraphLine(line);

        Graph::TVertex vertex_from = vertices_in_line[0];
        vertices.push_back(vertex_from);
        for (auto i = 1; i < vertices_in_line.size(); ++i)
        {
            Graph::TVertex vertex_to = vertices_in_line[i];
            edges.emplace_back(vertex_from, vertex_to);
        }
    }

    return std::make_unique<Graphs::Graph>(vertices, edges);
}

Graphs::TGraphTopology GraphsIO::ReadGraphTopologyFromStream(std::istream& i_stream)
{
    Graphs::TGraphTopology topology;

    std::string line;
    while (!i_stream.eof())
    {
        std::getline(i_stream, line);
        if (line.empty())
            continue;

        auto vertex_and_point = _ReadTopologyLine(line);
        topology.insert(vertex_and_point);
    }

    return topology;
}