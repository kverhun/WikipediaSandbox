#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"
#include "../Graphs/Topology.h"

#include "../GraphClusterization/Clusterization.h"

#include <memory>
#include <istream>
#include <ostream>

namespace GraphsIO
{
    std::unique_ptr<Graphs::Graph> GRAPHSIO_API ReadGraphFromStream(std::istream& i_stream);
    Graphs::TGraphTopology         GRAPHSIO_API ReadGraphTopologyFromStream(std::istream& i_stream);
    
    std::map<Graphs::Graph::TVertex, std::string> GRAPHSIO_API ReadGraphVerticesDescriptionFromStream(std::istream& i_stream);

    GraphClusterization::TClusterMap GRAPHSIO_API ReadClusterizationFromStream(std::istream& i_stream);
    void GRAPHSIO_API WriteClusterizationToStream(const GraphClusterization::TClusterMap& i_clusterization, std::ostream& o_stream);
}