#pragma once

#include "Point2d.h"
#include "Rectangle2d.h"

#include <vector>

namespace Geometry
{
    class ITopology
    {
    public:
        using TPointList = std::vector<Point2d>;

        virtual TPointList GetPointsInRectangle(const Rectangle2d& i_rectangle) const = 0;
    };
}