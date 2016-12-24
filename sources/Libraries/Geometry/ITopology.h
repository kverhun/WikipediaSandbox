#pragma once

#include "Point2d.h"
#include "Rectangle2d.h"

#include <vector>
#include <map>

namespace Geometry
{
    class ITopology
    {
    public:
        using TPointID = size_t;
        using TPoints = std::map<TPointID, Point2d>;
        using TPointList = std::vector<Point2d>;

        virtual ~ITopology() = 0 {}
        virtual const TPoints& GetPoints() const = 0;
        virtual const TPointList& GetPointList() const = 0;
        virtual TPoints GetPointsInRectangle(const Rectangle2d& i_rectangle) const = 0;
    };
}