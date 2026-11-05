cmake_minimum_required(VERSION 3.16)
project(OutagePredictor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)
add_executable(outage_predictor ${SOURCES})
target_include_directories(outage_predictor PRIVATE include)