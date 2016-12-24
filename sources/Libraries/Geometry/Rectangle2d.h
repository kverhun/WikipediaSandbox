#pragma once

#include "Point2d.h"

namespace Geometry
{
    class Rectangle2d
    {
    public:
        Rectangle2d(const Point2d& i_point_min, const Point2d& i_point_max);

        const Point2d& GetPointMin() const;
        const Point2d& GetPointMax() const;

    private:
        Point2d m_point_min;
        Point2d m_point_max;
        
    };
}