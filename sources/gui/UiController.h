#pragma once

#include "../Libraries/Geometry/Point2d.h"
#include "../Libraries/Geometry/ITopology.h"

#include "../Libraries/Graphs/Graph.h"
#include "../Libraries/Graphs/Topology.h"

#include "../Libraries/GraphClusterization/Clusterization.h"

#include <memory>
#include <set>

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
    const Geometry::ITopology& GetTopology() const;
    const TGraphDescription& GetGraphDescription() const;

    const std::vector<Geometry::Point2d>& GetTopologyPoints() const;
    const std::pair<Geometry::Point2d, Geometry::Point2d>& GetTopologyBoundingBox() const;

    void SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region);

    double GetPointRadius() const;

private:
    void _GenerateClusterizations();
    struct _GraphInfo
    {
        TGraphPtr mp_graph;
        std::unique_ptr<Geometry::ITopology> mp_topology;
        TDescriptionPtr mp_description;
        std::unique_ptr<GraphClusterization::Clusterization> mp_clusterization;
    };
    const _GraphInfo& _GetAppropriateGraph(double i_zoom_factor) const;

private:
    TGraphPtr mp_graph;
    TDescriptionPtr mp_description;

    // zoom_factor * 10
    std::map<size_t, std::unique_ptr<_GraphInfo>> m_clusterization;
    std::map<size_t, double> m_zoom_to_point_radius;

    double m_current_zoom_factor;

    std::set<std::unique_ptr<GraphClusterization::Clusterization>> m_clusterizations;
};