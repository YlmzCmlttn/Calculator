cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CalculatorCore LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    CalculatorCore
    src/Calculator.cpp
)
target_include_directories(CalculatorCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
