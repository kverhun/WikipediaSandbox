#include "Rectangle2d.h"

using namespace Geometry;

Rectangle2d::Rectangle2d(const Point2d& i_point_min, const Point2d& i_point_max)
    : m_point_min(i_point_min)
    , m_point_max(i_point_max)
{}

const Point2d& Rectangle2d::GetPointMin() const
{
    return m_point_min;
}

const Point2d& Rectangle2d::GetPointMax() const
{
    return m_point_max;
}