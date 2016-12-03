cmake_minimum_required(VERSION 2.8.9)

add_library(Graphs SHARED
    _Library.h
    Graph.h
    Graph.cpp
    GraphGenerator.h
    GraphGenerator.cpp
)
target_compile_definitions(Graphs PRIVATE GRAPHS_LIBRARY)