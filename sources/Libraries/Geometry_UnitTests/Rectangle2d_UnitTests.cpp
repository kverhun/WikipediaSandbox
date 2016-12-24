#include <catch.hpp>

#include "../Geometry/Point2d.h"
#include "../Geometry/Utils.h"
#include "../Geometry/Rectangle2d.h"

using namespace Geometry;


TEST_CASE("Rectangle2dTests")
{
    SECTION("RectangleCreation1")
    {
        Point2d pt1{ 1., 2. };
        Point2d pt2{ 5., 6. };
        Rectangle2d rectangle(pt1, pt2);

        REQUIRE(AreEqual2d(pt1, rectangle.GetPointMin()));
        REQUIRE(AreEqual2d(pt2, rectangle.GetPointMax()));
    }
}

TEST_CASE("Rectangle2dContaintsPointTests")
{
    SECTION("Test1")
    {
        Point2d pt1{ 1., 2. };
        Point2d pt2{ 5., 6. };
        Rectangle2d rectangle(pt1, pt2);

        Point2d inside1{ 2., 3. };
        Point2d inside2{ 4.5, 2.1 };
        Point2d inside3{ 4.98, 2.01 };

        REQUIRE(rectangle.DoesContainPoint(inside1));
        REQUIRE(rectangle.DoesContainPoint(inside2));
        REQUIRE(rectangle.DoesContainPoint(inside3));
    }
}