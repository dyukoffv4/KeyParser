cmake_minimum_required(VERSION 3.16)
project(Keyparser VERSION 1.0.1 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(keyparser SHARED src/binds.cpp src/parser.cpp src/tasks.cpp src/key.cpp)
