#pragma once

#include "../Libraries/Geometry/Point2d.h"

#include <QWidget>

class SceneWidget : public QWidget
{
public:
    SceneWidget(QWidget* ip_parent);

private:
    void paintEvent(QPaintEvent* ip_event) override;

private:
    std::vector<Geometry::Point2d> m_points;

};