#include "SceneWidget.h"

#include "../Libraries/Geometry/Utils.h"

#include <QPainter>

using Geometry::Point2d;

namespace
{
    const std::vector<Point2d> g_points = {
        Point2d(1., 1.), Point2d(1., 2.), Point2d(2., 2.)
    };

    const size_t g_margin = 10;
    const size_t g_point_radius = 3;
}

SceneWidget::SceneWidget(QWidget* ip_parent)
    : QWidget(ip_parent)
    , m_points(g_points)
{
    setStyleSheet("background-color: white");
}

void SceneWidget::paintEvent(QPaintEvent* ip_event)
{
    auto points_to_draw = m_points;
    auto points_boundaries = Geometry::GetPointsBoundaries(points_to_draw);

    for (auto& pt : points_to_draw)
    {
        pt.SetX(pt.GetX() - points_boundaries.first.GetX());
        pt.SetY(pt.GetY() - points_boundaries.first.GetY());
    }

    Point2d widget_size{
        static_cast<double>(width()) - 2 * g_margin, 
        static_cast<double>(height()) - 2 * g_margin
    };

    auto dx = points_boundaries.second.GetX() - points_boundaries.first.GetX();
    auto dy = points_boundaries.second.GetY() - points_boundaries.first.GetY();
    
    auto scale = std::max(
        dx / widget_size.GetX(),
        dy / widget_size.GetY()
        );

    for (auto& pt : points_to_draw)
    {
        pt.SetX(pt.GetX() / scale);
        pt.SetY(pt.GetY() / scale);
    }

    for (auto& pt : points_to_draw)
    {
        pt.SetX(pt.GetX() + g_margin);
        pt.SetY(pt.GetY() + g_margin);
    }

    QPainter painter(this);
    painter.setBrush(Qt::blue);

    for (const auto& pt : points_to_draw)
        painter.drawEllipse(QPoint(pt.GetX(), pt.GetY()), g_point_radius, g_point_radius);
}

