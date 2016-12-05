#pragma once

#include "Graph.h"
#include "../Geometry/Point2d.h"

#include <map>

namespace Graphs
{
    using TGraphTopology = std::map<Graph::TVertex, Geometry::Point2d>;
}