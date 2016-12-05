#pragma once

#include "_Library.h"
#include "../Graphs/Graph.h"

#include <memory>
#include <istream>

namespace GraphsIO
{
    std::unique_ptr<Graphs::Graph> GRAPHSIO_API ReadGraphFromStream(std::istream& i_stream);
}