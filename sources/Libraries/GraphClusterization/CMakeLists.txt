cmake_minimum_required(VERSION 2.8.9)

add_library(GraphClusterization SHARED
    _Library.h
    Clusterization.h
    Clusterization.cpp
    RandomClusterization.h
    RandomClusterization.cpp
)
target_compile_definitions(GraphClusterization PRIVATE GRAPHCLUSTERIZATION_LIBRARY)
target_link_libraries(GraphClusterization Graphs)