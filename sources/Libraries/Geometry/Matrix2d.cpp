#include "Matrix2d.h"

using namespace Geometry;

Matrix2d::Matrix2d(double i_11, double i_12, double i_21, double i_22)
    : m_11(i_11), m_12(i_12), m_21(i_21), m_22(i_22)
{}

Point2d Matrix2d::PreMultiply(const Point2d& i_point) const
{
    double res_x = m_11 * i_point.GetX() + m_12 * i_point.GetY();
    double res_y = m_21 * i_point.GetX() + m_22 * i_point.GetY();
    return Geometry::Point2d(res_x, res_y);
}

Matrix2d Matrix2d::Inverse() const
{
    double det = m_11*m_22 - m_12*m_21;
    return Matrix2d(m_22 / det, -m_12 / det, -m_21 / det, m_11 / det);
}
