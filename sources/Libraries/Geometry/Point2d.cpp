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