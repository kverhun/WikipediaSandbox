#include "SceneWidget.h"

#include "../Libraries/Geometry/Point2d.h"
#include "../Libraries/Geometry/Utils.h"

#include "../Libraries/Graphs/Graph.h"
#include "../Libraries/Graphs/GraphGenerator.h"

#include <QPainter>
#include <QFont>
#include <QDesktopServices>

#include <QWheelEvent>

#include <iostream>
#include <string>

using Geometry::Point2d;
using Graphs::Graph;

namespace
{
    const std::vector<Point2d> g_points = {
        Point2d(1., 1.), Point2d(1., 2.), Point2d(2., 2.), Point2d(3., 1.)
    };

    const Graphs::Graph g_graph({ 1, 2, 3, 4 }, { {1, 2}, {1, 3}, {2, 4} });
    const std::map<Graphs::Graph::TVertex, Point2d> g_graph_on_plane{ { 1, Point2d(1., 1.) }, { 2, Point2d(1., 2.) }, { 3, Point2d(2., 2.) }, {4, Point2d(3., 1.) } };

    const size_t g_margin = 10;

    const double g_zoom_factor = 0.1;

    const std::unique_ptr<Graphs::Graph> gp_random_graph = Graphs::GenerateGraph(10000);

    void _OutputPoint(const std::string& i_description, const Geometry::Point2d& i_point)
    {
        std::cout << i_description << ": [" << i_point.GetX() << "; " << i_point.GetY() << "]." << std::endl;
    }

    const size_t g_point_radius = 10;
    const size_t g_highlighted_point_radius = 15;

    const QColor g_nodes_color(150, 150, 255, 96);
    const QColor g_adjacent_node_color(119, 128, 255, 192);
    const QColor g_picked_node_color(28, 43, 255, 236);

    const QColor g_edge_color(255, 221, 221);
    const QColor g_highlighted_edge_color(158, 55, 55);

    const QColor g_text_color(24, 14, 58);

    const Graph::TVertex g_invalid_vertex = static_cast<Graph::TVertex>(-1);
}

class SceneWidget::_Scene
{
public:
    _Scene(const UiController& i_controller)
        : m_controller(i_controller)
    { }

    const std::vector<Geometry::Point2d>& GetPoints() const
    {
        return m_controller.GetTopologyPoints();
    }

    const Geometry::ITopology& GetTopology() const
    {
        return m_controller.GetTopology();
    }

    std::vector<std::pair<Point2d, Point2d>> GetSegments() const
    {
        std::vector<std::pair<Point2d, Point2d>> segments;

        for (const auto& edge : m_controller.GetGraph().GetEdges())
        {
            auto it_first = m_controller.GetTopology().GetPoints().find(edge.first);
            auto it_second = m_controller.GetTopology().GetPoints().find(edge.second);
            if (it_first == m_controller.GetTopology().GetPoints().end() || it_second == m_controller.GetTopology().GetPoints().end())
                continue;

            Point2d pt_from(it_first->second);
            Point2d pt_to(it_second->second);
            segments.emplace_back(pt_from, pt_to);
        }

        return segments;
    }

    std::pair<Geometry::Point2d, Geometry::Point2d> GetBoundingBox() const
    {
        return m_controller.GetTopologyBoundingBox();
    }

    void SetPickedVertices(const Graph::TVertices& i_vertices)
    {
        m_picked_vertexes = i_vertices;
        m_highlighted_vertexes.clear();
        m_highlighted_edges.clear();

        if (!m_picked_vertexes.empty())
        { 
            m_highlighted_edges = m_controller.GetGraph().GetEdgesFromVertex(m_picked_vertexes.front());
            for (const auto& e : m_highlighted_edges)
                m_highlighted_vertexes.push_back(e.second);
        }

        if (!m_picked_vertexes.empty())
        {
            std::cout << "Pick: " << m_picked_vertexes.front() << std::endl;
            std::cout << "Adjacent: ";
            for (const auto& v : m_highlighted_vertexes)
                std::cout << v << " ";
            std::cout << std::endl;
            std::cout << "Edges: ";
            for (const auto& e : m_highlighted_edges)
                std::cout << "[" << e.first << ", " << e.second << "]; ";
            std::cout << std::endl;
        }
    }

    const std::vector<Geometry::Point2d> GetPickedPoints() const
    {
        return _GetPointsForVertices(m_picked_vertexes);
    }

    const std::vector<Geometry::Point2d> GetHighlightedPoints() const
    {
        return _GetPointsForVertices(m_highlighted_vertexes);
    }

    std::vector<std::pair<Point2d, Point2d>> GetHighlightedSegments() const
    {
        std::vector<std::pair<Point2d, Point2d>> highlighted_segments;
        if (m_picked_vertexes.empty())
            return highlighted_segments;

        auto point_from = m_controller.GetTopology().GetPoints().at(m_picked_vertexes.front());

        for (const auto& e : m_highlighted_edges)
            highlighted_segments.emplace_back(point_from, m_controller.GetTopology().GetPoints().at(e.second));

        return highlighted_segments;
    }

    std::vector<std::pair<Point2d, std::string>> GetTitlesToDraw() const
    {
        std::vector<std::pair<Point2d, std::string>> res;
        for (const auto& c : { m_picked_vertexes, m_highlighted_vertexes })
            for (const auto& v : c)
            {
                std::string description = "---";
                // temporary check for not written descriptions (due to enc oding problems)
                try
                {
                    description = m_controller.GetGraphDescription().at(v);
                }
                catch (...)
                { }
                res.emplace_back(m_controller.GetTopology().GetPoints().at(v), description);
            }
        return res;
    }

    bool IsVertexHighlighted(const Graph::TVertex& i_vertex) const
    {
        bool is_picked = (std::find(m_picked_vertexes.begin(), m_picked_vertexes.end(), i_vertex) != m_picked_vertexes.end());
        bool is_highlighted = (std::find(m_highlighted_vertexes.begin(), m_highlighted_vertexes.end(), i_vertex) != m_highlighted_vertexes.end());
        return is_picked || is_highlighted;
    }

    std::string GetDescription(const Graph::TVertex& i_vertex) const
    {
        std::string description = "";
        try
        {
            description = m_controller.GetGraphDescription().at(i_vertex);
        }
        catch(...){}
        return description;
    }

private:
    std::vector<Geometry::Point2d> _GetPointsForVertices(const Graph::TVertices& i_vertices) const
    {
        std::vector<Geometry::Point2d> points;
        for (auto v : i_vertices)
            points.push_back(m_controller.GetTopology().GetPoints().at(v));
        return points;
    }

private:
    const UiController& m_controller;

    Graph::TVertices m_picked_vertexes;
    Graph::TVertices m_highlighted_vertexes;
    Graphs::Graph::TEdges m_highlighted_edges;
};


class SceneWidget::_Panner
{
public:
    _Panner(const QPoint& i_start_point_screen)
        : m_current_point(i_start_point_screen)
    { }

    ~_Panner()
    {
    
    }

    QPoint MoveAndGetDelta(const QPoint& i_new_point)
    {
        QPoint delta = m_current_point - i_new_point;
        m_current_point = i_new_point;
        return delta;
    }

private:
    QPoint m_current_point;

};


SceneWidget::SceneWidget(QWidget* ip_parent, std::unique_ptr<UiController> ip_controller)
    : QWidget(ip_parent)
    , mp_controller(std::move(ip_controller))
    , mp_scene(std::make_unique<_Scene>(*mp_controller.get()))
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

    auto draw_points_on_screen = [this](const std::vector<Geometry::Point2d>& i_points_to_draw, QPainter& io_painter, QColor i_color, int i_radius)
    {
        std::vector<QPoint> points_to_draw_screen;
        points_to_draw_screen.reserve(i_points_to_draw.size());

        for (const auto& pt : i_points_to_draw)
            points_to_draw_screen.push_back(_TransformPointFromWorldToWidget(pt));

        io_painter.setBrush(i_color);

        for (const auto& pt : points_to_draw_screen)
            io_painter.drawEllipse(pt, i_radius, i_radius);
    };
    
    auto draw_segments_on_screen = [this](const std::vector<std::pair<Point2d, Point2d>>& i_segments, QPainter& io_painter, QColor i_color)
    {
        std::vector<std::pair<QPoint, QPoint>> segments_to_draw_screen;
        for (const auto& segment : i_segments)
            segments_to_draw_screen.emplace_back(_TransformPointFromWorldToWidget(segment.first), _TransformPointFromWorldToWidget(segment.second));

        io_painter.setPen(i_color);
        for (const auto& segment : segments_to_draw_screen)
            io_painter.drawLine(segment.first, segment.second);
    };

    QPainter painter(this);

    draw_segments_on_screen(mp_scene->GetSegments(), painter, g_edge_color);

    //auto radius_x_screen = QPoint(g_point_radius, 0);
    //auto radius_x_world = _TransformPointFromWidgetToWorld(radius_x_screen) - _TransformPointFromWidgetToWorld(QPoint(0, 0));

    auto radius = mp_controller->GetPointRadius();
    Point2d radius_world{ radius, 0 };
    auto radius_screen = _TransformPointFromWorldToWidget(radius_world) - _TransformPointFromWorldToWidget(Point2d{0,0});

    const size_t point_radius = std::max(radius_screen.x(), radius_screen.y());


    auto points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetPoints(), m_current_region);
    if (!points_to_draw.empty())
        draw_points_on_screen(points_to_draw, painter, g_nodes_color, point_radius);

    std::cout << "points_to_draw.size(): " << points_to_draw.size() << std::endl;

    auto selected_points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetPickedPoints(), m_current_region);
    if (!selected_points_to_draw.empty())
        draw_points_on_screen(selected_points_to_draw, painter, g_picked_node_color, point_radius);

    auto highlighted_points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetHighlightedPoints(), m_current_region);
    if (!highlighted_points_to_draw.empty())
        draw_points_on_screen(highlighted_points_to_draw, painter, g_adjacent_node_color, point_radius);

    auto highlighted_segments = mp_scene->GetHighlightedSegments();
    draw_segments_on_screen(highlighted_segments, painter, g_highlighted_edge_color);

    auto draw_text_near_point = [this](const Point2d& i_point, const QString& i_str, QPainter& io_painter, QColor i_color)
    {
        auto point_screen = _TransformPointFromWorldToWidget(i_point);
        QPoint text_point(point_screen.x() - 2 * g_highlighted_point_radius, point_screen.y() - 2 * g_highlighted_point_radius);

        QFont font("Times", 15);
        io_painter.setPen(i_color);
        io_painter.setFont(font);
        io_painter.drawText(text_point, i_str);
    };

    auto titles_to_draw = mp_scene->GetTitlesToDraw();
    for (const auto& title : titles_to_draw)
        draw_text_near_point(title.first, QString::fromStdString(title.second), painter, g_text_color);
}

void SceneWidget::wheelEvent(QWheelEvent* ip_event)
{
    std::cout << "Wheel event. Delta = " << ip_event->delta() << std::endl;

    auto fracs = Geometry::GetRegionsFraction(m_current_region, mp_scene->GetBoundingBox());
    auto visible_x_frac = fracs.first;
    auto visible_y_frac = fracs.second;
    if ((std::min(visible_x_frac, visible_y_frac) <= 0.1 && ip_event->delta() > 0) || (std::max(visible_x_frac, visible_y_frac) >= 2. && ip_event->delta() < 0))
    {
        std::cout << "x frac: " << visible_x_frac << "; y_frac: " << visible_y_frac << std::endl;
        std::cout << "Min/max zoom reached" << std::endl;
        return;
    }

    if (ip_event->delta() > 0)
        _ZoomIn();
    else
        _ZoomOut();
}


void SceneWidget::mouseReleaseEvent(QMouseEvent* ip_event)
{
    bool should_perform_pick = (mp_panner == nullptr);

    if (mp_panner)
        mp_panner.reset();

    if (!should_perform_pick)
        return;

    auto picked_vertex = _GetVertexUnderCursor(ip_event->pos());

    if (picked_vertex != g_invalid_vertex)
        mp_scene->SetPickedVertices({ picked_vertex });
    else
        mp_scene->SetPickedVertices({});
    
    update();
}

Graphs::Graph::TVertex SceneWidget::_GetVertexUnderCursor(const QPoint& i_point_screen) const
{
    auto screen_pos = i_point_screen;
    auto world_pos = _TransformPointFromWidgetToWorld(screen_pos);

    auto it_point_closest_to_pointer = std::min_element(mp_scene->GetTopology().GetPoints().begin(), mp_scene->GetTopology().GetPoints().end(),
        [world_pos](const std::pair<Graph::TVertex, Point2d>& i_entry1, const std::pair<Graph::TVertex, Point2d>& i_entry2)
    {
        return Geometry::DistanceSquare(world_pos, i_entry1.second) < Geometry::DistanceSquare(world_pos, i_entry2.second);
    });
    auto point_closest_to_pointer = it_point_closest_to_pointer->second;

    auto radius = mp_controller->GetPointRadius();
    Point2d radius_world{ radius, 0 };
    auto radius_screen = _TransformPointFromWorldToWidget(radius_world) - _TransformPointFromWorldToWidget(Point2d{ 0,0 });
    const size_t point_radius = std::max(radius_screen.x(), radius_screen.y());

    auto radius_x_screen = QPoint(point_radius, 0);
    auto radius_x_world = _TransformPointFromWidgetToWorld(radius_x_screen) - _TransformPointFromWidgetToWorld(QPoint(0, 0));

    auto radius_y_screen = QPoint(0, point_radius);
    auto radius_y_world = _TransformPointFromWidgetToWorld(radius_y_screen) - _TransformPointFromWidgetToWorld(QPoint(0, 0));

    auto rx_world = std::sqrt(Geometry::DistanceSquare(Point2d{ 0,0 }, radius_x_world));
    auto ry_world = std::sqrt(Geometry::DistanceSquare(Point2d{ 0,0 }, radius_y_world));

    auto dx_world = point_closest_to_pointer.GetX() - world_pos.GetX();
    auto dy_world = point_closest_to_pointer.GetY() - world_pos.GetY();

    bool picked = [&]()
    {
        if (dx_world*dx_world / rx_world / rx_world + dy_world*dy_world / ry_world / ry_world <= 1)
            return true;
        else
            return false;
    }();
    if (picked)
        return it_point_closest_to_pointer->first;
    else
        return g_invalid_vertex;
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

    bool pan = (ip_event->buttons() == Qt::LeftButton);
    if (pan)
    {
        if (mp_panner)
        {
            auto delta_screen = mp_panner->MoveAndGetDelta(ip_event->pos());
            auto delta_world = _TransformPointFromWidgetToWorld(delta_screen);
            auto zero_world = _TransformPointFromWidgetToWorld(QPoint(0, 0));
            std::cout << "Pan" << std::endl;
            std::cout << "Delta screen: " << delta_screen.x() << "; " << delta_screen.y() << std::endl;
            _OutputPoint("delta_world", delta_world);

            m_current_region.first += (delta_world - zero_world);
            m_current_region.second += (delta_world - zero_world);

            _OutputPoint("Current region min", m_current_region.first);
            _OutputPoint("Current region max", m_current_region.second);

            update();
        }
        else
            mp_panner = std::make_unique<_Panner>(ip_event->pos());
    }

}

void SceneWidget::mouseDoubleClickEvent(QMouseEvent* ip_event)
{
    auto picked_vertex = _GetVertexUnderCursor(ip_event->pos());
    if (mp_scene->IsVertexHighlighted(picked_vertex))
    {
        QString url = "https://wikipedia.org/wiki/";
        QString description = QString::fromStdString(mp_scene->GetDescription(picked_vertex));
        description.replace(' ', '_');

        url.append(description);

        QDesktopServices::openUrl(QUrl(url));
    }
}

void SceneWidget::_ExpandCurrentRegion(double i_dx, double i_dy)
{
    m_current_region.first.SetX(m_current_region.first.GetX() - i_dx / 2);
    m_current_region.second.SetX(m_current_region.second.GetX() + i_dx / 2);
    m_current_region.first.SetY(m_current_region.first.GetY() - i_dy / 2);
    m_current_region.second.SetY(m_current_region.second.GetY() + i_dy / 2);

    mp_controller->SetVisibleRegion(m_current_region);
}

void SceneWidget::_ShrinkCurrentRegion(double i_dx, double i_dy)
{
    m_current_region.first.SetX(m_current_region.first.GetX() + i_dx / 2);
    m_current_region.second.SetX(m_current_region.second.GetX() - i_dx / 2);
    m_current_region.first.SetY(m_current_region.first.GetY() + i_dy / 2);
    m_current_region.second.SetY(m_current_region.second.GetY() - i_dy / 2);

    mp_controller->SetVisibleRegion(m_current_region);
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