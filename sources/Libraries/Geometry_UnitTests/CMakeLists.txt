cmake_minimum_required(VERSION 2.8.9)

add_executable(Geometry_UnitTests 
    PointComparison_UnitTests.cpp
    Rectangle2d_UnitTests.cpp
    )
add_dependencies(Geometry_UnitTests catch)
target_link_libraries(Geometry_UnitTests Geometry)