#include "UiController.h"

#include "../Libraries/Geometry/Utils.h"

namespace
{
    using Geometry::Point2d;

    std::map<Graphs::Graph::TVertex, Point2d> _GenerateRandomGraphPoints(const Graphs::Graph& i_graph)
    {
        std::map<Graphs::Graph::TVertex, Point2d> res;
        std::vector<Point2d> random_points = Geometry::GenerateRandomPoints(i_graph.GetVertices().size(), -100000, -100000, 100000, 100000);
        for (size_t i = 0; i < random_points.size(); ++i)
        {
            res.insert(std::make_pair(i_graph.GetVertices()[i], random_points[i]));
        }
        return res;
    }
}

UiController::UiController(
    TGraphPtr ip_graph,
    TTopologyPtr ip_topology,
    TDescriptionPtr ip_description)
    : mp_graph(ip_graph)
    , mp_topology((!ip_topology || ip_topology->empty()) ? std::make_shared<Graphs::TGraphTopology>(_GenerateRandomGraphPoints(*mp_graph.get())) : ip_topology)
    , mp_description(ip_description)
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