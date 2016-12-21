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

UiController::UiController(
    TGraphPtr ip_graph,
    TTopologyPtr ip_topology,
    TDescriptionPtr ip_description)
    : mp_graph(ip_graph)
    , mp_description(ip_description)

{
    _GenerateClusterizations();
}

UiController::~UiController()
{

}

const Graphs::Graph& UiController::GetGraph() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_graph.get();
}

const Graphs::TGraphTopology& UiController::GetTopology() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_topology.get();
}

const TGraphDescription& UiController::GetGraphDescription() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_description.get();
}

const std::vector<Geometry::Point2d>& UiController::GetTopologyPoints() const
{
    return _GetAppropriateGraph(m_current_zoom_factor).m_topology_points;
}

const std::pair<Geometry::Point2d, Geometry::Point2d>& UiController::GetTopologyBoundingBox() const
{
    static auto box = Geometry::GetPointsBoundaries(m_clusterization.crbegin()->second->m_topology_points);
    return box;
}

void UiController::SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region)
{
    auto fracs = Geometry::GetRegionsFraction(i_region, GetTopologyBoundingBox());
    m_current_zoom_factor = std::max(fracs.first, fracs.second);

    std::cout << "Current zoom: " << m_current_zoom_factor << std::endl;
}

double UiController::GetPointRadius() const
{
    size_t zoom_factor_10(m_current_zoom_factor * 10);
    std::cout << "zoom_factor_10: " << zoom_factor_10 << std::endl;
    for (auto it = m_zoom_to_point_radius.cbegin(); it != m_zoom_to_point_radius.cend(); ++it)
        if (zoom_factor_10 > it->first && (std::next(it) == m_zoom_to_point_radius.cend() || zoom_factor_10 < std::next(it)->first))
        {
            std::cout << "Point radius: " << it->second << std::endl;
            return it->second;
        }
    throw std::logic_error("invalid zoom factor");
}

void UiController::_GenerateClusterizations()
{
    m_zoom_to_point_radius = 
    {
        {1, 250}, {5, 500}, {10, 1250}, {15, 2500}, {20, 7500}
    };

    auto base_graph_size = mp_graph->GetVertices().size();
    
    const std::map<size_t, size_t> g_cluster_sizes = { 
        {1, base_graph_size },
        {5, base_graph_size / 4}, 
        {10, base_graph_size / 16}, 
        {15, base_graph_size / 64}, 
        {20, base_graph_size / 256}
    };

    for (const auto& e : g_cluster_sizes)
        m_clusterization[e.first] = std::make_unique<_GraphInfo>();

    m_clusterization.rbegin()->second->mp_graph = mp_graph;
    m_clusterization.rbegin()->second->mp_description = mp_description;

    for (auto it = ++m_clusterization.rbegin(); it != m_clusterization.rend(); ++it)
    {
        auto number_of_vertices = g_cluster_sizes.at(it->first);
        it->second->mp_clusterization = GraphClusterization::CreateRandomClusterization(*std::prev(it)->second->mp_graph.get(), number_of_vertices);
        it->second->mp_graph = it->second->mp_clusterization->GetClusterGraph();
    }

    m_clusterization.begin()->second->mp_topology = std::make_shared<Graphs::TGraphTopology>(_GenerateRandomGraphPoints(*m_clusterization.begin()->second->mp_graph.get()));
    for (auto it = ++m_clusterization.begin(); it != m_clusterization.end(); ++it)
    {
        it->second->mp_topology = std::make_shared<Graphs::TGraphTopology>(_GenerateRandomGraphPointsBasedOnClusterization(*std::prev(it)->second->mp_clusterization.get(), *std::prev(it)->second->mp_topology.get()));
        it->second->m_topology_points = _RetrieveMapValues(*it->second->mp_topology.get());
    }
}

const UiController::_GraphInfo& UiController::_GetAppropriateGraph(double i_zoom_factor) const
{
    size_t zoom_factor_10(i_zoom_factor * 10);
    for (auto it = m_clusterization.cbegin(); it != m_clusterization.cend(); ++it)
        if (zoom_factor_10 > it->first && (std::next(it) == m_clusterization.cend() || zoom_factor_10 < std::next(it)->first))
            return *it->second.get();
    throw std::logic_error("invalid zoom factor");
}