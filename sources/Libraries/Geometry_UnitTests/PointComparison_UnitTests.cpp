#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../Geometry/Point2d.h"
#include "../Geometry/Utils.h"

using Geometry::Point2d;

TEST_CASE("PointsComparison")
{
    SECTION("AreEqual1")
    {
        Point2d pt1{ 1.0, 1.0 };
        Point2d pt2{ 1.0, 1.0 };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual2")
    {
        Point2d pt1{ 100500., 250.};
        Point2d pt2{ 100500., 250. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual3")
    {
        Point2d pt1{ 125., 100250. };
        Point2d pt2{ 125., 100250. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual4")
    {
        Point2d pt1{ -200., 750000. };
        Point2d pt2{ -200., 750000. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual5")
    {
        Point2d pt1{ 0.002, 0.0005 };
        Point2d pt2{ 0.002, 0.0005 };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual6")
    {
        Point2d pt1{ -0.002, 0.0005 };
        Point2d pt2{ -0.002, 0.0005 };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreEqual7")
    {
        Point2d pt1{ -1001., -100500. };
        Point2d pt2{ -1001., -100500. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(true == are_equal);
    }

    SECTION("AreNotEqual1")
    {
        Point2d pt1{ 1., 1.};
        Point2d pt2{ 1., 1.00001 };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(false == are_equal);
    }

    SECTION("AreNotEqual2")
    {
        Point2d pt1{ 0., -0.001 };
        Point2d pt2{ 0., 0. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(false == are_equal);
    }

    SECTION("AreNotEqual3")
    {
        Point2d pt1{ 1000.1, 250. };
        Point2d pt2{ 1000. , 250. };

        bool are_equal = Geometry::AreEqual2d(pt1, pt2);
        REQUIRE(false == are_equal);
    }
}