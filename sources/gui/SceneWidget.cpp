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
    _ZoomOut();
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

    std::vector<QPoint> points_to_draw_screen;
    points_to_draw.reserve(points_to_draw.size());

    for (const auto& pt : points_to_draw)
        points_to_draw_screen.push_back(_TransformPointFromWorldToWidget(pt));

    QPainter painter(this);
    painter.setBrush(Qt::blue);

    for (const auto& pt : points_to_draw_screen)
        painter.drawEllipse(pt, g_point_radius, g_point_radius);
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
        auto world_point = _TransformPointFromWidgetToWorld(ip_event->pos());
        std::string msg = "Window coors: [" + std::to_string(pos_x) + "; " + std::to_string(pos_y) + "], " 
            + "World coords: [" + std::to_string(world_point.GetX()) + "; " + std::to_string(world_point.GetY()) + "]";
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


Geometry::Point2d SceneWidget::_TransformPointFromWidgetToWorld(const QPoint& i_point) const
{
    auto scene_to_world_matrix = _GetTransformMatrixFromSceneToWorld();
    Geometry::Point2d point_to_transform(static_cast<double>(i_point.x()), static_cast<double>(i_point.y()));
    auto transformed_point = scene_to_world_matrix.PreMultiply(point_to_transform);
    Geometry::Point2d shift(m_current_region.first.GetX(), m_current_region.second.GetY());
    transformed_point += shift;
    return transformed_point;
}

QPoint SceneWidget::_TransformPointFromWorldToWidget(const Geometry::Point2d& i_point) const
{
    auto world_to_widget_matrix = _GetTransformMatrixFromSceneToWorld().Inverse();
    Geometry::Point2d shift(m_current_region.first.GetX(), m_current_region.second.GetY());
    auto res = world_to_widget_matrix.PreMultiply(i_point - shift);
    return QPoint(static_cast<int>(res.GetX()), static_cast<int>(res.GetY()));
}

Geometry::Matrix2d SceneWidget::_GetTransformMatrixFromSceneToWorld() const
{
    double a11 = (m_current_region.second.GetX() - m_current_region.first.GetX())  / size().width();
    double a22 = -(m_current_region.second.GetY() - m_current_region.first.GetY()) / size().height();
    return Geometry::Matrix2d(a11, 0, 0, a22);
}