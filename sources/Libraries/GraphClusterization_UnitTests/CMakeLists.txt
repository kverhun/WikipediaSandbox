cmake_minimum_required(VERSION 2.8.9)

add_executable(GraphClusterization_UnitTests
    GraphClusterization_UnitTests.cpp
    BFS_UnitTests.cpp
    )
add_dependencies(GraphClusterization_UnitTests catch)
target_link_libraries(GraphClusterization_UnitTests GraphClusterization)