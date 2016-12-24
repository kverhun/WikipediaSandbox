cmake_minimum_required(VERSION 2.8.9)

add_library(Geometry SHARED
    _Library.h
    Point2d.h
    Point2d.cpp
    Matrix2d.h
    Matrix2d.cpp
    Utils.h
    Utils.cpp
    Rectangle2d.h
    Rectangle2d.cpp
    ITopology.h
    TopologySimple.h
    TopologySimple.cpp
)
target_compile_definitions(Geometry PRIVATE GEOMETRY_LIBRARY)