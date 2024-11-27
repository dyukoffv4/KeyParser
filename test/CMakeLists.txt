cmake_minimum_required(VERSION 3.16)
project(KeyparserTests LANGUAGES C CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/..)

add_executable(test src/test.cpp)
