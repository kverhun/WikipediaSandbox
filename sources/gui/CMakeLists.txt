cmake_minimum_required(VERSION 2.8.9)

# Qt
set(CMAKE_PREFIX_PATH "D:/dev/Qt/5.7/msvc2015_64")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Core REQUIRED)

add_executable(WikipediaSandboxGUI 
    main.cpp
    SceneWidget.h
    SceneWidget.cpp)
    
target_link_libraries(WikipediaSandboxGUI Qt5::WinMain)
qt5_use_modules(WikipediaSandboxGUI Widgets)

target_link_libraries(WikipediaSandboxGUI Geometry)