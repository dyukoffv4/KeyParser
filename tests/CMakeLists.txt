cmake_minimum_required(VERSION 3.16)

find_package(Catch2 3 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/..)

add_executable(test_key src/key.cpp ../src/key.cpp)
target_link_libraries(test_key PRIVATE Catch2::Catch2WithMain)

add_executable(test_parser src/parser.cpp ../src/parser.cpp)
target_link_libraries(test_parser PRIVATE Catch2::Catch2WithMain)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
