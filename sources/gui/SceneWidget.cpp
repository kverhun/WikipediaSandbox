#include "SceneWidget.h"

#include "../Libraries/Geometry/Point2d.h"
#include "../Libraries/Geometry/Utils.h"

#include "../Libraries/Graphs/Graph.h"
#include "../Libraries/Graphs/GraphGenerator.h"

#include <QPainter>
#include <QFont>

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

    std::map<Graphs::Graph::TVertex, Point2d> _GenerateRandomGraphPoints(const Graphs::Graph& i_graph)
    {
        std::map<Graphs::Graph::TVertex, Point2d> res;
        std::vector<Point2d> random_points = Geometry::GenerateRandomPoints(i_graph.GetVertices().size(), -100000, -100000, 100000, 100000);
        for (size_t i = 0; i < random_points.size(); ++i)
        {
            res.insert(std::make_pair(i_graph.GetVertices()[i], random_points[i]));
        }
        return res;
    }

    const std::unique_ptr<Graphs::Graph> gp_random_graph = Graphs::GenerateGraph(10000);
    const std::map<Graphs::Graph::TVertex, Point2d> g_random_graph_map = _GenerateRandomGraphPoints(*gp_random_graph.get());

    void _OutputPoint(const std::string& i_description, const Geometry::Point2d& i_point)
    {
        std::cout << i_description << ": [" << i_point.GetX() << "; " << i_point.GetY() << "]." << std::endl;
    }

    template<typename K, typename V>
    std::vector<V> _RetrieveMapValues(const std::map<K, V>& i_map)
    {
        std::vector<V> result;
        std::transform(i_map.begin(), i_map.end(), std::back_inserter(result), [&i_map](const std::pair<K,V>& i_el) {return i_el.second; });
        return std::move(result);
    }

    const size_t g_point_radius = 7;
    const size_t g_highlighted_point_radius = 10;

    const QColor g_nodes_color(150, 150, 255, 96);
    const QColor g_adjacent_node_color(119, 128, 255, 192);
    const QColor g_picked_node_color(28, 43, 255, 236);

    const QColor g_edge_color(255, 221, 221);
    const QColor g_highlighted_edge_color(158, 55, 55);

    const QColor g_text_color(24, 14, 58);
}

class SceneWidget::_Scene
{
public:
    _Scene(std::shared_ptr<Graphs::Graph> ip_graph, std::shared_ptr<Graphs::TGraphTopology> ip_topology, std::shared_ptr<std::map<Graphs::Graph::TVertex, std::string>> ip_description)
        : mp_graph(ip_graph)
        , mp_graph_topology( (!ip_topology || ip_topology->empty()) ? std::make_shared<Graphs::TGraphTopology>(_GenerateRandomGraphPoints(*mp_graph.get())) : ip_topology)
        , m_points(_RetrieveMapValues(*mp_graph_topology.get()))
        , m_bounding_box(Geometry::GetPointsBoundaries(m_points))
        , mp_description(ip_description)
    { }

    const std::vector<Geometry::Point2d>& GetPoints() const
    {
        return m_points;
    }

    const Graphs::TGraphTopology& GetTopology() const
    {
        return *mp_graph_topology.get();
    }

    std::vector<std::pair<Point2d, Point2d>> GetSegments() const
    {
        std::vector<std::pair<Point2d, Point2d>> segments;

        for (const auto& edge : mp_graph->GetEdges())
        {
            auto it_first = mp_graph_topology->find(edge.first);
            auto it_second = mp_graph_topology->find(edge.second);
            if (it_first == mp_graph_topology->end() || it_second == mp_graph_topology->end())
                continue;

            Point2d pt_from(it_first->second);
            Point2d pt_to(it_second->second);
            segments.emplace_back(pt_from, pt_to);
        }

        return segments;
    }

    std::pair<Geometry::Point2d, Geometry::Point2d> GetBoundingBox() const
    {
        return m_bounding_box;
    }

    void SetPickedVertices(const Graph::TVertices& i_vertices)
    {
        m_picked_vertexes = i_vertices;
        m_highlighted_vertexes.clear();
        m_highlighted_edges.clear();

        if (!m_picked_vertexes.empty())
        { 
            m_highlighted_edges = mp_graph->GetEdgesFromVertex(m_picked_vertexes.front());
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

        auto point_from = mp_graph_topology->at(m_picked_vertexes.front());

        for (const auto& e : m_highlighted_edges)
            highlighted_segments.emplace_back(point_from, mp_graph_topology->at(e.second));

        return highlighted_segments;
    }

    std::vector<std::pair<Point2d, std::string>> GetTitlesToDraw() const
    {
        std::vector<std::pair<Point2d, std::string>> res;
        for (const auto& c : { m_picked_vertexes, m_highlighted_vertexes })
            for (const auto& v : c)
            {
                std::string description = "---";
                // temporary check for not written descriptions (due to encoding problems)
                try
                {
                    description = mp_description->at(v);
                }
                catch (...)
                { }
                res.emplace_back(mp_graph_topology->at(v), description);
            }
        return res;
    }

private:
    std::vector<Geometry::Point2d> _GetPointsForVertices(const Graph::TVertices& i_vertices) const
    {
        std::vector<Geometry::Point2d> points;
        for (auto v : i_vertices)
            points.push_back(mp_graph_topology->at(v));
        return points;
    }

private:

    std::shared_ptr<Graphs::Graph> mp_graph;
    std::shared_ptr<Graphs::TGraphTopology> mp_graph_topology;
    std::vector<Geometry::Point2d> m_points;
    std::pair<Geometry::Point2d, Geometry::Point2d> m_bounding_box;
    std::shared_ptr<std::map<Graphs::Graph::TVertex, std::string>> mp_description;

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


SceneWidget::SceneWidget(QWidget* ip_parent, std::shared_ptr<Graphs::Graph> ip_graph, std::shared_ptr<Graphs::TGraphTopology> ip_topology,
    std::shared_ptr<std::map<Graphs::Graph::TVertex, std::string>> ip_description)
    : QWidget(ip_parent)
    , mp_scene(std::make_unique<_Scene>(ip_graph, ip_topology, ip_description))
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

    auto points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetPoints(), m_current_region);
    if (!points_to_draw.empty())
        draw_points_on_screen(points_to_draw, painter, g_nodes_color, g_point_radius);


    auto selected_points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetPickedPoints(), m_current_region);
    if (!selected_points_to_draw.empty())
        draw_points_on_screen(selected_points_to_draw, painter, g_picked_node_color, g_highlighted_point_radius);

    auto highlighted_points_to_draw = Geometry::FilterPointsByBoundingBox(mp_scene->GetHighlightedPoints(), m_current_region);
    if (!highlighted_points_to_draw.empty())
        draw_points_on_screen(highlighted_points_to_draw, painter, g_adjacent_node_color, g_highlighted_point_radius);

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
    if (ip_event->delta() > 0)
        _ZoomIn();
    else
        _ZoomOut();
}

void SceneWidget::mousePressEvent(QMouseEvent* ip_event)
{
    mouseMoveEvent(ip_event);
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* ip_event)
{
    if (mp_panner)
        mp_panner.reset();

    auto screen_pos = ip_event->pos();
    auto world_pos = _TransformPointFromWidgetToWorld(screen_pos);

    auto it_point_closest_to_pointer = std::min_element(mp_scene->GetTopology().begin(), mp_scene->GetTopology().end(), 
        [world_pos](const std::pair<Graph::TVertex, Point2d>& i_entry1, const std::pair<Graph::TVertex, Point2d>& i_entry2)
    {
        return Geometry::DistanceSquare(world_pos, i_entry1.second) < Geometry::DistanceSquare(world_pos, i_entry2.second);
    });
    auto point_closest_to_pointer = it_point_closest_to_pointer->second;
    
    auto radius_x_screen = QPoint(g_point_radius, 0);
    auto radius_x_world = _TransformPointFromWidgetToWorld(radius_x_screen) - _TransformPointFromWidgetToWorld(QPoint( 0,0 ));

    auto radius_y_screen = QPoint(0, g_point_radius);
    auto radius_y_world = _TransformPointFromWidgetToWorld(radius_y_screen) - _TransformPointFromWidgetToWorld(QPoint( 0,0 ));

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
        mp_scene->SetPickedVertices({ it_point_closest_to_pointer->first });
    else
        mp_scene->SetPickedVertices({});
    
    update();
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