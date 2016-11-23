#include "Utils.h"

#include <algorithm>
#include <iterator>

namespace
{
    bool _IsPointInsideBoundingBox(const Geometry::Point2d& i_point, const std::pair<Geometry::Point2d, Geometry::Point2d>& i_bounding_box)
    {
        return
            i_bounding_box.first.GetX() <= i_point.GetX() && i_point.GetX() <= i_bounding_box.second.GetX() &&
            i_bounding_box.first.GetY() <= i_point.GetY() && i_point.GetY() <= i_bounding_box.second.GetY();
    }
}

namespace Geometry
{
    std::pair<Point2d, Point2d> GetPointsBoundaries(const std::vector<Point2d>& i_points)
    {
        Point2d pt_min{i_points.front().GetX(), i_points.front().GetY()};
        Point2d pt_max{ pt_min.GetX(), pt_min.GetY() };

        for (const auto& pt : i_points)
        {
            if (pt.GetX() < pt_min.GetX())
                pt_min.SetX(pt.GetX());
            if (pt.GetX() > pt_max.GetX())
                pt_max.SetX(pt.GetX());
            if (pt.GetY() < pt_min.GetY())
                pt_min.SetY(pt.GetY());
            if (pt.GetY() > pt_max.GetY())
                pt_max.SetY(pt.GetY());
        }

        return std::make_pair(pt_min, pt_max);
    }

    std::vector<Point2d> FilterPointsByBoundingBox(const std::vector<Point2d>& i_points, const std::pair<Point2d, Point2d>& i_bounding_box)
    {
        std::vector<Point2d> filtered_points;
        
        std::copy_if(i_points.begin(), i_points.end(), std::back_inserter(filtered_points), [&i_bounding_box](const Point2d& i_point) {return _IsPointInsideBoundingBox(i_point, i_bounding_box); });

        return filtered_points;
    }
}