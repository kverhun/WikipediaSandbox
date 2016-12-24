#pragma once

#include "_Library.h"
#include "Point2d.h"

namespace Geometry
{
    class GEOMETRY_API Rectangle2d
    {
    public:
        Rectangle2d(const Point2d& i_point_min, const Point2d& i_point_max);

        const Point2d& GetPointMin() const;
        const Point2d& GetPointMax() const;

        bool DoesContainPoint(const Point2d& i_point) const;

    private:
        Point2d m_point_min;
        Point2d m_point_max;
        
    };
}