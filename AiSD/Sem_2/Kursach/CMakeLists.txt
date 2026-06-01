cmake_minimum_required(VERSION 4.2)
project(RRT)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")

find_package(SDL3 REQUIRED)

add_executable(RRT ${SRC_FILES})

target_link_libraries(RRT PRIVATE SDL3::SDL3)
