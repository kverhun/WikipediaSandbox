#pragma once

#include "../Libraries/Geometry/Point2d.h"

#include <memory>

#include <QWidget>

class SceneWidget : public QWidget
{
public:
    SceneWidget(QWidget* ip_parent);

private:
    void paintEvent(QPaintEvent* ip_event) override;
    void wheelEvent(QWheelEvent* ip_event) override;

    void _ZoomIn();
    void _ZoomOut();
    void _ExpandCurrentRegion(double i_dx, double i_dy);
    void _ShrinkCurrentRegion(double i_dx, double i_dy);

private:
    class _Scene;
    std::unique_ptr<_Scene> mp_scene;

    std::pair<Geometry::Point2d, Geometry::Point2d> m_current_region;
};