#pragma once

#include "../Libraries/Geometry/Point2d.h"

#include "../Libraries/Graphs/Graph.h"
#include "../Libraries/Graphs/Topology.h"

#include <memory>

using TGraphDescription = std::map<Graphs::Graph::TVertex, std::string>;

using TGraphPtr = std::shared_ptr<Graphs::Graph>;
using TTopologyPtr = std::shared_ptr<Graphs::TGraphTopology>;
using TDescriptionPtr = std::shared_ptr<TGraphDescription>;

class UiController
{
public:
    UiController(
        TGraphPtr ip_graph,
        TTopologyPtr ip_topology,
        TDescriptionPtr ip_description);

    const Graphs::Graph& GetGraph() const;
    const Graphs::TGraphTopology& GetTopology() const;
    const TGraphDescription& GetGraphDescription() const;

private:
    TGraphPtr mp_graph;
    TTopologyPtr mp_topology;
    TDescriptionPtr mp_description;

};