cmake_minimum_required(VERSION 3.5)

project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(./Calculator CalculatorCore_build)

add_executable(Calculator main.cpp)

target_link_libraries(Calculator PRIVATE CalculatorCore)