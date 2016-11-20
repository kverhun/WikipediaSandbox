#pragma once

#include "_Library.h"

#include "Point2d.h"

#include <vector>

namespace Geometry
{
    std::pair<Point2d, Point2d> GEOMETRY_API GetPointsBoundaries(const std::vector<Point2d>& i_points);
}