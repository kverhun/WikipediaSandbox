#include <catch.hpp>

#include "../Geometry/ITopology.h"
#include "../Geometry/TopologySimple.h"

#include <algorithm>

using Geometry::Point2d;
using Geometry::Rectangle2d;
using Geometry::ITopology;

namespace
{
    const ITopology::TPointList g_test_points1 = { 
        {1,1}, {2,1}, {3,1},
        {1,2}, {2,2}, {3,2},
        {1,3}, {2,3}, {3,3}
    };

    const Rectangle2d g_test_rectangle_11{ Point2d{1.5, 1.5}, Point2d{2.5, 2.5} };
    const std::vector<ITopology::TPointID> g_test_ids_11 = { 4 };
    
    const Rectangle2d g_test_rectangle_12{ Point2d{ -1, -1 }, Point2d{ 4, 4} };
    const std::vector<ITopology::TPointID> g_test_ids_12 = { 0,1,2,3,4,5,6,7,8 };

    const Rectangle2d g_test_rectangle_13{ Point2d{ 1.25, 1.25 }, Point2d{ 3.5, 3.5 } };
    const std::vector<ITopology::TPointID> g_test_ids_13 = { 4,5,7,8 };

    const Rectangle2d g_test_rectangle_14{ Point2d{ -2, -2 }, Point2d{ 8, 2.45 } };
    const std::vector<ITopology::TPointID> g_test_ids_14 = { 0,1,2,3,4,5 };

    const Rectangle2d g_test_rectangle_15{ Point2d{ 10, 5 }, Point2d{ 24, 72 } };
    const std::vector<ITopology::TPointID> g_test_ids_15 = { };

    ITopology::TPoints _GeneratePointsFromPointList(const ITopology::TPointList& i_point_list)
    {
        ITopology::TPoints points;
        for (size_t i = 0; i < i_point_list.size(); ++i)
            points.insert({ i, i_point_list[i] });
        return points;
    }

    bool _ComparePointsToIDList(const ITopology::TPoints& i_points, const std::vector<ITopology::TPointID>& i_id_list)
    {
        std::vector<ITopology::TPointID> id_list_sorted(i_id_list.begin(), i_id_list.end());
        std::sort(id_list_sorted.begin(), id_list_sorted.end());

        std::vector<ITopology::TPointID> points_converted_to_ids;
        for (const auto& entry : i_points)
            points_converted_to_ids.push_back(entry.first);
        std::sort(points_converted_to_ids.begin(), points_converted_to_ids.end());

        return id_list_sorted == points_converted_to_ids;
    }
}

TEST_CASE("TopologySimple")
{
    auto p_topology = Geometry::CreateSimpleTopology(_GeneratePointsFromPointList(g_test_points1));
    SECTION("Test1")
    {
        REQUIRE(_ComparePointsToIDList(p_topology->GetPointsInRectangle(g_test_rectangle_11), g_test_ids_11));
    }
    SECTION("Test2")
    {
        REQUIRE(_ComparePointsToIDList(p_topology->GetPointsInRectangle(g_test_rectangle_12), g_test_ids_12));
    }
    SECTION("Test3")
    {
        REQUIRE(_ComparePointsToIDList(p_topology->GetPointsInRectangle(g_test_rectangle_13), g_test_ids_13));
    }
    SECTION("Test4")
    {
        REQUIRE(_ComparePointsToIDList(p_topology->GetPointsInRectangle(g_test_rectangle_14), g_test_ids_14));
    }
    SECTION("Test5")
    {
        REQUIRE(_ComparePointsToIDList(p_topology->GetPointsInRectangle(g_test_rectangle_15), g_test_ids_15));
    }
}