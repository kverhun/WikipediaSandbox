#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"
#include "../Graphs/Topology.h"

#include <memory>
#include <istream>

namespace GraphsIO
{
    std::unique_ptr<Graphs::Graph> GRAPHSIO_API ReadGraphFromStream(std::istream& i_stream);
    Graphs::TGraphTopology         GRAPHSIO_API ReadGraphTopologyFromStream(std::istream& i_stream);
    
    std::map<Graphs::Graph::TVertex, std::string> GRAPHSIO_API ReadGraphVerticesDescriptionFromStream(std::istream& i_stream);
}