#pragma once

#include "_Library.h"

#include "ITopology.h"

#include <memory>

namespace Geometry
{
    std::unique_ptr<ITopology> GEOMETRY_API CreateSimpleTopology(const ITopology::TPoints& i_points);
}