#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../GraphClusterization/Clusterization.h"
#include "../Graphs/Graph.h"

using namespace GraphClusterization;
using Graphs::Graph;

TEST_CASE("CreateClusterization")
{
	SECTION("DummyTest")
	{
        Graph g{ {1,2}, {{1, 2}} };
        TClusterMap cluster_map{ {1,1}, {2,2} };

        Clusterization clusterization{ cluster_map, g };
        REQUIRE(nullptr == clusterization.GetClusterGraph());
	}
}