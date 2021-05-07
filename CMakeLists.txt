cmake_minimum_required(VERSION 3.5)

project(Calculator LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -mavx -rdynamic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mavx -rdynamic")


add_subdirectory(./Calculator CalculatorCore_build)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(Calculator
 main.cpp
 mainwindow.cpp
 mainwindow.h
 )

target_link_libraries(Calculator PRIVATE Qt5::Widgets CalculatorCore)