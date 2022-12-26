cmake_minimum_required(VERSION 3.20.0)
project(spiceroad)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(spicelib)
add_subdirectory(app)
