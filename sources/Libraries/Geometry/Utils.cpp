#include "Utils.h"

#include <algorithm>
#include <iterator>
#include <random>
#include <limits>

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
        Point2d pt_min{ i_points.front().GetX(), i_points.front().GetY() };
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

    std::vector<Point2d> GenerateRandomPoints(size_t i_number, int i_xfrom, int i_yfrom, int i_xto, int i_yto)
    {
        std::vector<Point2d> points;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> x_distr(i_xfrom, i_xto);
        std::uniform_int_distribution<> y_distr(i_yfrom, i_yto);

        for (size_t i = 0; i < i_number; ++i)
        {
            int x = x_distr(rd);
            int y = y_distr(rd);
            points.push_back(Point2d(x, y));
        }

        return points;
    }

    double DistanceSquare(const Point2d& i_point1, const Point2d& i_point2)
    {
        auto dx = i_point1.GetX() - i_point2.GetX();
        auto dy = i_point1.GetY() - i_point2.GetY();
        return dx*dx + dy*dy;
    }

    std::pair<double, double> GetRegionsFraction(const std::pair<Point2d, Point2d>& i_first_region, const std::pair<Point2d, Point2d>& i_second_region)
    {
        auto first_reg_size = i_first_region.second - i_first_region.first;
        auto first_reg_x = abs(first_reg_size.GetX());
        auto first_reg_y = abs(first_reg_size.GetY());

        auto second_reg_size = i_second_region.second - i_second_region.first;
        auto second_reg_x = abs(second_reg_size.GetX());
        auto second_reg_y = abs(second_reg_size.GetY());

        auto x_frac = first_reg_x / second_reg_x;
        auto y_frac = first_reg_y / second_reg_y;

        return std::make_pair(x_frac, y_frac);
    }

    bool AreEqual2d(const Point2d& i_point1, const Point2d& i_point2)
    {
        const double epsilon = std::numeric_limits<double>::epsilon();
        const double dx = std::abs(i_point1.GetX() - i_point2.GetX());
        const double dy = std::abs(i_point1.GetY() - i_point2.GetY());
        if (dx < epsilon && dy < epsilon)
            return true;
        else
            return false;
    }

}