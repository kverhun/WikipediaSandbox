#include "Point2d.h"

using namespace Geometry;

Point2d::Point2d(double i_x, double i_y)
    : m_x(i_x)
    , m_y(i_y)
{ }

double Point2d::GetX() const
{
    return m_x;
}

void Point2d::SetX(double i_x)
{
    m_x = i_x;
}

double Point2d::GetY() const
{
    return m_y;
}

void Point2d::SetY(double i_y)
{
    m_y = i_y;
}

Point2d& Point2d::operator+=(const Point2d& i_other)
{
    m_x += i_other.m_x;
    m_y += i_other.m_y;
    return *this;
}

Point2d& Point2d::operator-=(const Point2d& i_other)
{
    return (*this) += (-i_other);
}

Point2d Point2d::operator-() const
{
    return Point2d(-m_x, -m_y);
}