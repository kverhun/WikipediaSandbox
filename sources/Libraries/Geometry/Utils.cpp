#include "Utils.h"

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
}