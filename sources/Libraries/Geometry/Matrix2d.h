#pragma once

#include "_Library.h"

#include "Point2d.h"

namespace Geometry
{
    class GEOMETRY_API Matrix2d
    {
    public:
        Matrix2d(double i_11, double i_12, double i_21, double i_22);

        Point2d PreMultiply(const Geometry::Point2d& i_point) const;

        Matrix2d Inverse() const;

    private:
        double m_11, m_12, m_21, m_22;
    };
}