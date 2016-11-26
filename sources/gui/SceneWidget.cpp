#include "SceneWidget.h"

#include "../Libraries/Geometry/Point2d.h"
#include "../Libraries/Geometry/Utils.h"

#include <QPainter>
#include <QWheelEvent>

#include <iostream>
#include <string>

using Geometry::Point2d;

namespace
{
    const std::vector<Point2d> g_points = {
        Point2d(1., 1.), Point2d(1., 2.), Point2d(2., 2.), Point2d(3., 1.)
    };

    const size_t g_margin = 10;
    const size_t g_point_radius = 3;

    const double g_zoom_factor = 0.1;

    void _OutputPoint(const std::string& i_description, const Geometry::Point2d& i_point)
    {
        std::cout << i_description << ": [" << i_point.GetX() << "; " << i_point.GetY() << "]." << std::endl;
    }
}

class SceneWidget::_Scene
{
public:
    _Scene()
        : m_points(g_points)
        , m_bounding_box(Geometry::GetPointsBoundaries(m_points))
    {
    }

    const std::vector<Geometry::Point2d>& GetPoints() const
    {
        return m_points;
    }

    std::pair<Geometry::Point2d, Geometry::Point2d> GetBoundingBox() const
    {
        return m_bounding_box;
    }

private:
    std::vector<Geometry::Point2d> m_points;
    std::pair<Geometry::Point2d, Geometry::Point2d> m_bounding_box;
};


SceneWidget::SceneWidget(QWidget* ip_parent)
    : QWidget(ip_parent)
    , mp_scene(std::make_unique<_Scene>())
    , m_current_region(mp_scene->GetBoundingBox())
{
    setStyleSheet(
        "border: 1px solid black;"
        "background-color: white;");
}

void SceneWidget::SetMouseMoveMessageDelegate(SceneWidget::TMessageDelegate i_message_delegate)
{
    m_message_delegate = i_message_delegate;
}

void SceneWidget::paintEvent(QPaintEvent* ip_event)
{
    auto points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetPoints(), m_current_region);
    if (points_to_draw.empty())
        return;

    //auto points_boundaries = Geometry::GetPointsBoundaries(points_to_draw);
    auto points_boundaries = m_current_region;

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

void SceneWidget::wheelEvent(QWheelEvent* ip_event)
{
    std::cout << "Wheel event. Delta = " << ip_event->delta() << std::endl;
    if (ip_event->delta() > 0)
        _ZoomIn();
    else
        _ZoomOut();
}

void SceneWidget::mousePressEvent(QMouseEvent* ip_event)
{
    mouseMoveEvent(ip_event);
}

void SceneWidget::mouseMoveEvent(QMouseEvent* ip_event)
{
    int pos_x = ip_event->pos().x();
    int pos_y = ip_event->pos().y();
    
    if (m_message_delegate)
    {
        std::string msg = "Window coors: [" + std::to_string(pos_x) + "; " + std::to_string(pos_y) + "]";
        m_message_delegate(msg);
    }
}

void SceneWidget::_ExpandCurrentRegion(double i_dx, double i_dy)
{
    m_current_region.first.SetX(m_current_region.first.GetX() - i_dx / 2);
    m_current_region.second.SetX(m_current_region.second.GetX() + i_dx / 2);
    m_current_region.first.SetY(m_current_region.first.GetY() - i_dy / 2);
    m_current_region.second.SetY(m_current_region.second.GetY() + i_dy / 2);
}

void SceneWidget::_ShrinkCurrentRegion(double i_dx, double i_dy)
{
    m_current_region.first.SetX(m_current_region.first.GetX() + i_dx / 2);
    m_current_region.second.SetX(m_current_region.second.GetX() - i_dx / 2);
    m_current_region.first.SetY(m_current_region.first.GetY() + i_dy / 2);
    m_current_region.second.SetY(m_current_region.second.GetY() - i_dy / 2);
}

void SceneWidget::_ZoomIn()
{
    const auto bbox = mp_scene->GetBoundingBox();
    const auto zoom_dx = (bbox.second.GetX() - bbox.first.GetX()) * g_zoom_factor;
    const auto zoom_dy = (bbox.second.GetY() - bbox.first.GetY()) * g_zoom_factor;
    _ShrinkCurrentRegion(zoom_dx, zoom_dy);
    
    _OutputPoint("Current region min", m_current_region.first);
    _OutputPoint("Current region max", m_current_region.second);

    update();
}

void SceneWidget::_ZoomOut()
{
    const auto bbox = mp_scene->GetBoundingBox();
    const auto zoom_dx = (bbox.second.GetX() - bbox.first.GetX()) * g_zoom_factor;
    const auto zoom_dy = (bbox.second.GetY() - bbox.first.GetY()) * g_zoom_factor;
    _ExpandCurrentRegion(zoom_dx, zoom_dy);
    
    _OutputPoint("Current region min", m_current_region.first);
    _OutputPoint("Current region max", m_current_region.second);

    update();
}
