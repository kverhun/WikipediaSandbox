cmake_minimum_required(VERSION 2.8.9)

add_library(GraphsIO SHARED
    _Library.h
    GraphIO.h
    GraphIO.cpp
)
target_compile_definitions(GraphsIO PRIVATE GRAPHSIO_LIBRARY)
target_link_libraries(GraphsIO Graphs)
target_link_libraries(GraphsIO Geometry)