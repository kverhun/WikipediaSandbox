#include "UiController.h"

#include "../Libraries/Geometry/Utils.h"
#include "../Libraries/GraphClusterization/Clusterization.h"
#include "../Libraries/GraphClusterization/RandomClusterization.h"

#include <map>
#include <iterator>
#include <algorithm>

namespace
{
    using Geometry::Point2d;

    Graphs::TGraphTopology _GenerateRandomPointsForVerticesInRectangle(
        const Graphs::Graph::TVertices& i_vertices,
        int i_x_min, int i_y_min,
        int i_x_max, int i_y_max
        )
    {
        Graphs::TGraphTopology res;
        std::vector<Point2d> random_points = Geometry::GenerateRandomPoints(i_vertices.size(), i_x_min, i_y_min, i_x_max, i_y_min);
        for (size_t i = 0; i < random_points.size(); ++i)
        {
            res.insert(std::make_pair(i_vertices[i], random_points[i]));
        }
        return res;
    }

    Graphs::TGraphTopology _GenerateRandomGraphPoints(const Graphs::Graph& i_graph)
    {
        return _GenerateRandomPointsForVerticesInRectangle(i_graph.GetVertices(), -100000, -100000, 100000, 100000);
    }

    Graphs::TGraphTopology _GenerateRandomGraphPointsBasedOnClusterization(const GraphClusterization::Clusterization& i_clusterization)
    {
        Graphs::TGraphTopology topology;
        
        return topology;
    }

    template<typename K, typename V>
    std::vector<V> _RetrieveMapValues(const std::map<K, V>& i_map)
    {
        std::vector<V> result;
        std::transform(i_map.begin(), i_map.end(), std::back_inserter(result), [&i_map](const std::pair<K, V>& i_el) {return i_el.second; });
        return std::move(result);
    }
}

class UiController::_ClusterizationInfo
{
public:
    _ClusterizationInfo(const Graphs::Graph& i_graph)
        : m_base_graph(i_graph)
    {
        mp_clusterization = GraphClusterization::CreateRandomClusterization(m_base_graph, 100);
        m_clusterization_topology = _GenerateRandomGraphPoints(*mp_clusterization->GetClusterGraph().get());


    }

private:
    const Graphs::Graph& m_base_graph;
    Graphs::TGraphTopology m_base_topology;

    std::unique_ptr<GraphClusterization::Clusterization> mp_clusterization;
    Graphs::TGraphTopology m_clusterization_topology;
};

UiController::UiController(
    TGraphPtr ip_graph,
    TTopologyPtr ip_topology,
    TDescriptionPtr ip_description)
    : mp_graph(ip_graph)
    , mp_topology((!ip_topology || ip_topology->empty()) ? std::make_shared<Graphs::TGraphTopology>(_GenerateRandomGraphPoints(*mp_graph.get())) : ip_topology)
    , mp_description(ip_description)
    , m_topology_points(_RetrieveMapValues(*mp_topology.get()))
    , m_topology_bounding_box(Geometry::GetPointsBoundaries(m_topology_points))
{

}

const Graphs::Graph& UiController::GetGraph() const
{
    return *mp_graph.get();
}

const Graphs::TGraphTopology& UiController::GetTopology() const
{
    return *mp_topology.get();
}

const TGraphDescription& UiController::GetGraphDescription() const
{
    return *mp_description.get();
}

const std::vector<Geometry::Point2d>& UiController::GetTopologyPoints() const
{
    return m_topology_points;
}

const std::pair<Geometry::Point2d, Geometry::Point2d>& UiController::GetTopologyBoundingBox() const
{
    return m_topology_bounding_box;
}

void UiController::SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region)
{
    auto fracs = Geometry::GetRegionsFraction(i_region, m_topology_bounding_box);

}