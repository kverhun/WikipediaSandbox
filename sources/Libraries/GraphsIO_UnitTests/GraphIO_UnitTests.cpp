#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../GraphsIO/GraphIO.h"

TEST_CASE("Test")
{
    REQUIRE(GraphsIO::Sum(1, 2) == 3);
}