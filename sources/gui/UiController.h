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
    ~UiController();

    const Graphs::Graph& GetGraph() const;
    const Graphs::TGraphTopology& GetTopology() const;
    const TGraphDescription& GetGraphDescription() const;

    const std::vector<Geometry::Point2d>& GetTopologyPoints() const;
    const std::pair<Geometry::Point2d, Geometry::Point2d>& GetTopologyBoundingBox() const;

    void SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region);

    double GetPointRadius() const;
private:
    TGraphPtr mp_graph;
    TTopologyPtr mp_topology;
    TDescriptionPtr mp_description;

    std::vector<Geometry::Point2d> m_topology_points;
    std::pair<Geometry::Point2d, Geometry::Point2d> m_topology_bounding_box;

    class _ClusterizationInfo;
    std::unique_ptr<_ClusterizationInfo> mp_clusterization;

    double m_current_zoom_factor;
};