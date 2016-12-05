#include "GraphIO.h"

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

    std::vector<Graph::TVertex> _ParseLine(const std::string& i_line)
    {
        std::vector<Graph::TVertex> res;

        auto tokens = _Tokenize(i_line);
        for (const auto& t : tokens)
            res.push_back(std::stoi(t));

        return res;
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
        auto vertices_in_line = _ParseLine(line);

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