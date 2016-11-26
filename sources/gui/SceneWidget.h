#pragma once

#include "../Libraries/Geometry/Point2d.h"
#include "../Libraries/Geometry/Matrix2d.h"

#include <memory>
#include <functional>
#include <string>

#include <QWidget>

class SceneWidget : public QWidget
{
public:
    SceneWidget(QWidget* ip_parent);

    using TMessageDelegate = std::function<void(const std::string&)>;
    void SetMouseMoveMessageDelegate(TMessageDelegate i_message_delegate);

private:
    void paintEvent(QPaintEvent* ip_event) override;
    void wheelEvent(QWheelEvent* ip_event) override;
    void mousePressEvent(QMouseEvent* ip_event) override;
    void mouseReleaseEvent(QMouseEvent* ip_event) override;
    void mouseMoveEvent(QMouseEvent* ip_event) override;

    void _ZoomIn();
    void _ZoomOut();
    void _ExpandCurrentRegion(double i_dx, double i_dy);
    void _ShrinkCurrentRegion(double i_dx, double i_dy);

    Geometry::Matrix2d _GetTransformMatrixFromSceneToWorld() const;
    Geometry::Point2d _TransformPointFromWidgetToWorld(const QPoint& i_point) const;
    QPoint _TransformPointFromWorldToWidget(const Geometry::Point2d& i_point) const;

private:
    class _Scene;
    std::unique_ptr<_Scene> mp_scene;

    class _Panner;
    std::unique_ptr<_Panner> mp_panner;

    std::pair<Geometry::Point2d, Geometry::Point2d> m_current_region;

    TMessageDelegate m_message_delegate = nullptr;
};