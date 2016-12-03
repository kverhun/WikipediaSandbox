#pragma once

#include "_Library.h"

#include "Point2d.h"

#include <vector>

namespace Geometry
{
    std::pair<Point2d, Point2d> GEOMETRY_API GetPointsBoundaries(const std::vector<Point2d>& i_points);
    std::vector<Point2d> GEOMETRY_API FilterPointsByBoundingBox(const std::vector<Point2d>& i_points, const std::pair<Point2d, Point2d>& i_bounding_box);

    std::vector<Point2d> GEOMETRY_API GenerateRandomPoints(size_t i_number, int i_xfrom, int i_yfrom, int i_xto, int i_yto);
}