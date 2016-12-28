#include "UiController.h"

#include "../Libraries/Geometry/Utils.h"
#include "../Libraries/Geometry/TopologyGridBased.h"

#include "../Libraries/GraphClusterization/Clusterization.h"
#include "../Libraries/GraphClusterization/RandomClusterization.h"
#include "../Libraries/GraphClusterization/GreedyClusterization.h"

#include "../Libraries/GraphsIO/GraphIO.h"

#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>

#include <QString>

namespace
{
    using Geometry::Point2d;

    const int g_xmin = -6000000;
    const int g_ymin = g_xmin;
    const int g_xmax = 6000000;
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

    Graphs::TGraphTopology _GenerateRandomGraphPointsBasedOnClusterization(const GraphClusterization::Clusterization& i_clusterization, const Graphs::TGraphTopology& i_clusterization_topology, size_t i_cluster_dim)
    {
        Graphs::TGraphTopology topology;
        
        for (const auto& v : i_clusterization.GetBaseGraph().GetVertices())
        {
            auto cluster = i_clusterization.GetClusterIdForVertex(v);
            auto cluster_point = i_clusterization_topology.at(cluster);

            int xmin = cluster_point.GetX() - i_cluster_dim;
            int ymin = cluster_point.GetY() - i_cluster_dim;
            int xmax = cluster_point.GetX() + i_cluster_dim;
            int ymax = cluster_point.GetY() + i_cluster_dim;

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

    template<typename K, typename V>
    const V& _GetMapElementCorrespondingToRange(const std::map<K, V>& i_map, const K& i_key)
    {
        for (auto it = i_map.begin(); it != i_map.end(); ++it)
        {
            auto next_it = std::next(it);
            if (next_it == i_map.end())
                return it->second;

            if (it->first <= i_key && i_key <= next_it->first)
                return it->second;
        }
        throw std::logic_error("Invalid input");
    }

    void _WriteClusterizationToFile(const std::string& i_file_name, const GraphClusterization::TClusterMap& i_clusters)
    {
        std::ofstream file_stream(i_file_name);
        GraphsIO::WriteClusterizationToStream(i_clusters, file_stream);
    }

}

UiController::UiController(
    TGraphPtr ip_graph,
    TTopologyPtr ip_topology,
    TDescriptionPtr ip_description,
    TClusterizationsPtr ip_clusterizations)
    : mp_graph(ip_graph)
    , mp_description(ip_description)

{
    _GenerateClusterizations(ip_clusterizations);
}

UiController::~UiController()
{

}

const Graphs::Graph& UiController::GetGraph() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_graph.get();
}

const Geometry::ITopology& UiController::GetTopology() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_topology.get();
}

const TGraphDescription& UiController::GetGraphDescription() const
{
    return *_GetAppropriateGraph(m_current_zoom_factor).mp_description.get();
}

const std::vector<Geometry::Point2d>& UiController::GetTopologyPoints() const
{
    return _GetAppropriateGraph(m_current_zoom_factor).mp_topology->GetPointList();
}

const Graphs::Graph& UiController::GetBaseGraph() const
{
    return *m_clusterization.begin()->second->mp_graph.get();
}

const Geometry::ITopology& UiController::GetBaseTopology() const
{
    return *m_clusterization.begin()->second->mp_topology.get();
}

const TGraphDescription& UiController::GetBaseGraphDescription() const
{
    return *m_clusterization.begin()->second->mp_description.get();
}

double UiController::GetBasePointRadius() const
{
    return m_zoom_to_point_radius.begin()->second;
}


const std::pair<Geometry::Point2d, Geometry::Point2d>& UiController::GetTopologyBoundingBox() const
{
    static auto box = Geometry::GetPointsBoundaries(m_clusterization.cbegin()->second->mp_topology->GetPointList());
    return box;
}

bool UiController::SetVisibleRegion(const std::pair<Geometry::Point2d, Geometry::Point2d>& i_region)
{

    auto fracs = Geometry::GetRegionsFraction(i_region, GetTopologyBoundingBox());
    double new_zoom_factor = std::max(fracs.first, fracs.second);
    
    const auto* p_graph_before = &_GetAppropriateGraph(m_current_zoom_factor);
    const auto* p_new_graph = &_GetAppropriateGraph(new_zoom_factor);

    m_current_zoom_factor = new_zoom_factor;

    std::cout << "Current zoom: " << m_current_zoom_factor << std::endl;

    if (p_new_graph == p_graph_before)
        return false;
    else
        return true;
}

bool UiController::IsCurrentGraphBase() const
{
    const auto* p_base_graph = &GetBaseGraph();
    const auto* p_current_graph = &GetGraph();
    return p_base_graph == p_current_graph;
}

double UiController::GetPointRadius() const
{
    size_t zoom_factor_10 = m_current_zoom_factor * 10;
    return _GetMapElementCorrespondingToRange(m_zoom_to_point_radius, zoom_factor_10);
}

void UiController::_GenerateClusterizations(TClusterizationsPtr ip_clusterizations)
{
    m_zoom_to_point_radius = 
    {
        {1, 5000}, {2, 25000}, {5, 75000}, {8, 200000}//, {10, 250000}//, {15, 500000}//, {20, 375000}
    };
    std::map<size_t, size_t> g_cluster_dims =
    {
        { 1, 5000 },{ 2, 500000 },{ 5, 750000 },{ 8, 1000000 }//,{ 10, 1000000 }//, {15, 2000000}//, {20, 10000000 }
    };

    auto base_graph_size = mp_graph->GetVertices().size();
    
    const std::map<size_t, size_t> g_cluster_sizes = { 
        {1, base_graph_size },
        {2, 10000},
        {5, 1000 },
        {8, 50},
        //{10, base_graph_size / 25000 },
        //{15, base_graph_size / 75000},
        //{20, base_graph_size / 256}
    };

    for (const auto& e : g_cluster_sizes)
        m_clusterization[e.first] = std::make_unique<_GraphInfo>();

    m_clusterization.begin()->second->mp_graph = mp_graph;
    m_clusterization.begin()->second->mp_description = mp_description;

    size_t current_clusterization = 0;

    for (auto it = ++m_clusterization.begin(); it != m_clusterization.end(); ++it)
    {
        auto number_of_vertices = g_cluster_sizes.at(it->first);
        
        if (!ip_clusterizations)
        {
            it->second->mp_clusterization = GraphClusterization::CreateGreedyClusterization(*std::prev(it)->second->mp_graph.get(), number_of_vertices);
            std::string fname = "cl_" + std::to_string(current_clusterization) + ".csv";
            _WriteClusterizationToFile(fname, it->second->mp_clusterization->GetClusterMap());
        }
        else
        {
            it->second->mp_clusterization = std::make_unique<GraphClusterization::Clusterization>((*ip_clusterizations)[current_clusterization], *std::prev(it)->second->mp_graph.get());
        }
        ++current_clusterization;

        it->second->mp_graph = it->second->mp_clusterization->GetClusterGraph();
        it->second->mp_description = std::make_unique<TGraphDescription>();
    }

    m_clusterization.rbegin()->second->mp_topology = Geometry::CreateGridBasedTopology(_GenerateRandomGraphPoints(*m_clusterization.rbegin()->second->mp_graph.get()));
    for (auto it = ++m_clusterization.rbegin(); it != m_clusterization.rend(); ++it)
    {
        auto graph_topology = _GenerateRandomGraphPointsBasedOnClusterization(*std::prev(it)->second->mp_clusterization.get(), std::prev(it)->second->mp_topology->GetPoints(), g_cluster_dims.at(std::prev(it)->first));
        it->second->mp_topology = Geometry::CreateGridBasedTopology(graph_topology);
    }
}

const UiController::_GraphInfo& UiController::_GetAppropriateGraph(double i_zoom_factor) const
{
    size_t zoom_factor_10 = i_zoom_factor * 10;
    return *_GetMapElementCorrespondingToRange(m_clusterization, zoom_factor_10).get();
}

const Graphs::Graph::TVertex& UiController::GetNodeByTextPrefix(const std::string& i_prefix) const
{
    for (const auto& e : GetBaseGraphDescription())
    {
        if (e.second.find(i_prefix) == 0)
            return e.first;
    }
    throw std::logic_error("No such node");
}

const Graphs::Graph::TVertex& UiController::GetNodeByFullMatch(const std::string& i_title) const
{
    for (const auto& e : GetBaseGraphDescription())
    {
        if (int x = QString::compare(QString::fromStdString(i_title), QString::fromStdString(e.second), Qt::CaseInsensitive) == 0)
            return e.first;
    }
    throw std::logic_error("No such node");
}