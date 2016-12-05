cmake_minimum_required(VERSION 2.8.9)


add_executable(GraphsIO_UnitTests GraphIO_UnitTests.cpp)
add_dependencies(GraphsIO_UnitTests catch)
target_link_libraries(GraphsIO_UnitTests Geometry)
target_link_libraries(GraphsIO_UnitTests Graphs)
target_link_libraries(GraphsIO_UnitTests GraphsIO)
