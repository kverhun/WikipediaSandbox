#pragma once

#include "_Library.h"
#include "ITopology.h"

#include <memory>

namespace Geometry
{
    std::unique_ptr<ITopology> GEOMETRY_API CreateGridBasedTopology(const ITopology::TPoints& i_points);
}