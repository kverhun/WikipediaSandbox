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

bool Rectangle2d::DoesContainPoint(const Point2d& i_point) const
{
    bool contains_x = (m_point_min.GetX() < i_point.GetX() && i_point.GetX() < m_point_max.GetX());
    bool contains_y = (m_point_min.GetY() < i_point.GetY() && i_point.GetY() < m_point_max.GetY());
    return contains_x && contains_y;
}