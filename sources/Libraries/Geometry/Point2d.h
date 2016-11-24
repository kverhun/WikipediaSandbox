#pragma once

#include "_Library.h"

namespace Geometry
{
    class GEOMETRY_API Point2d
    {
    public:
        Point2d(double i_x, double i_y);

        double GetX() const;
        void SetX(double i_x);

        double GetY() const;
        void SetY(double i_y);

        Point2d& operator += (const Point2d& i_other);
        Point2d& operator -= (const Point2d& i_other);
        Point2d operator-() const;

    private:
        double m_x;
        double m_y;
    };
}
