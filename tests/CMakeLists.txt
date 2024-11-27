cmake_minimum_required(VERSION 3.16)

find_package(Catch2 3 REQUIRED)

add_executable(tests src/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
