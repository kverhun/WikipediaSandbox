#include "UiController.h"

#include "../Libraries/Geometry/Utils.h"

#include "../Libraries/GraphClusterization/Clusterization.h"
#include "../Libraries/GraphClusterization/RandomClusterization.h"

#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

namespace
{
    using Geometry::Point2d;

    const int g_xmin = -100000;
    const int g_ymin = g_xmin;
    const int g_xmax = 100000;
    const int g_ymax = g_xmax;

    const size_t g_number_of_clusters = 50;
    //const int g_cluster_dim = static_cast<int>(std::sqrt(g_xmax - g_xmin) / g_number_of_clusters);
    const int g_cluster_dim = 10000;

    Graphs::TGraphTopology _GenerateRandomPointsForVerticesInRectangle(
        const Graphs::Graph::TVertices& i_vertices,
        int i_x_min, int i_y_min,
        int i_x_max, int i_y_max
        )
    {
        Graphs::TGraphTopology res;
        std::vector<Point2d> random_points = Geometry::GenerateRandomPoints(i_vertices.size(), i_x_min, i_y_min, i_x_max, i_y_max);
        for (size_t i = 0; i < random_points.size(); ++i)
        {
            res.insert(std::make_pair(i_vertices[i], random_points[i]));
        }
        return res;
    }

    Graphs::TGraphTopology _GenerateRandomGraphPoints(const Graphs::Graph& i_graph)
    {
        return _GenerateRandomPointsForVerticesInRectangle(i_graph.GetVertices(), g_xmin, g_ymin, g_xmax, g_ymax);
    }

    Graphs::TGraphTopology _GenerateRandomGraphPointsBasedOnClusterization(const GraphClusterization::Clusterization& i_clusterization, const Graphs::TGraphTopology& i_clusterization_topology)
    {
        Graphs::TGraphTopology topology;
        
        for (const auto& v : i_clusterization.GetBaseGraph().GetVertices())
        {
            auto cluster = i_clusterization.GetClusterIdForVertex(v);
            auto cluster_point = i_clusterization_topology.at(cluster);

            int xmin = cluster_point.GetX() - g_cluster_dim;
            int ymin = cluster_point.GetY() - g_cluster_dim;
            int xmax = cluster_point.GetX() + g_cluster_dim;
            int ymax = cluster_point.GetY() + g_cluster_dim;

            auto points = _GenerateRandomPointsForVerticesInRectangle({ v }, xmin, ymin, xmax, ymax);
            topology.insert(*points.begin());
        }

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
        mp_clusterization = GraphClusterization::CreateRandomClusterization(m_base_graph, g_number_of_clusters);
        m_clusterization_topology = _GenerateRandomGraphPoints(*mp_clusterization->GetClusterGraph().get());
        m_clusterization_topology_points = _RetrieveMapValues(m_clusterization_topology);
        mp_clusterization_desciption = std::make_shared<TGraphDescription>();

        m_base_topology = _GenerateRandomGraphPointsBasedOnClusterization(*mp_clusterization.get(), m_clusterization_topology);
        m_base_topology_points = _RetrieveMapValues(m_base_topology);
    }

    const Graphs::TGraphTopology& GetBaseGraphTopology() const
    {
        return m_base_topology;
    }

    const std::vector<Geometry::Point2d>& GetBaseGraphTopologyPoints() const
    {
        return m_base_topology_points;
    }

    const Graphs::Graph& GetClusterizedGraph() const
    {
        return *mp_clusterization->GetClusterGraph().get();
    }

    const Graphs::TGraphTopology& GetClusterizedTopology() const
    {
        return m_clusterization_topology;
    }

    const std::vector<Geometry::Point2d>& GetClusterizedTopologyPoints() const
    {
        return m_clusterization_topology_points;
    }

    const TGraphDescription& GetClusterizationDescription() const
    {
        return *mp_clusterization_desciption.get();
    }

private:
    const Graphs::Graph& m_base_graph;
    Graphs::TGraphTopology m_base_topology;
    std::vector<Geometry::Point2d> m_base_topology_points;

    std::unique_ptr<GraphClusterization::Clusterization> mp_clusterization;
    Graphs::TGraphTopology m_clusterization_topology;
    std::vector<Geometry::Point2d> m_clusterization_topology_points;
    TDescriptionPtr mp_clusterization_desciption;
};

UiController::UiController(
    TGraphPtr ip_graph,
    TTopologyPtr ip_topology,
    TDescriptionPtr ip_description)
    : mp_graph(ip_graph)
    , mp_description(ip_description)
    , mp_clusterization(std::make_unique<_ClusterizationInfo>(*ip_graph.get()))
    , m_topology_bounding_box(Geometry::GetPointsBoundaries(mp_clusterization->GetBaseGraphTopologyPoints()))
{
}

UiController::~UiController()
{

}

const Graphs::Graph& UiController::GetGraph() const
{
    if (m_current_zoom_factor < 0.95)
    return *mp_graph.get();
    else
        return mp_clusterization->GetClusterizedGraph();
}

const Graphs::TGraphTopology& UiController::GetTopology() const
{
    if (m_current_zoom_factor <= 0.95)
        return mp_clusterization->GetBaseGraphTopology();
    else
        return mp_clusterization->GetClusterizedTopology();
}

const TGraphDescription& UiController::GetGraphDescription() const
{
    if (m_current_zoom_factor <= 0.95)
    return *mp_description.get();
    else
        return mp_clusterization->GetClusterizationDescription();
}

const std::vector<Geometry::Point2d>& UiController::GetTopologyPoints() const
{
    if (m_current_zoom_factor <= 0.95)
        return mp_clusterization->GetBaseGraphTopologyPoints();
    else
        return mp_clusterization->GetClusterizedTopologyPoints();
}

const std::pair<Geometry::Point2d, Geometry::Point2d>& UiController::GetTopologyBoundingBox() const
{
    return m_topology_bounding_box;
}

void UiController::SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region)
{
    auto fracs = Geometry::GetRegionsFraction(i_region, m_topology_bounding_box);
    m_current_zoom_factor = std::max(fracs.first, fracs.second);

    std::cout << "Current zoom: " << m_current_zoom_factor << std::endl;
}

double UiController::GetPointRadius() const
{
    if (m_current_zoom_factor <= 0.95)
        //return 200000. / 175;
        return 750;
    else
        return g_cluster_dim;
}