cmake_minimum_required(VERSION 3.15)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(simple_test src/simple_test.cpp)
add_executable(ray_test src/ray_test.cpp)
add_executable(sphere_test src/sphere_test.cpp)
add_executable(world_test src/world_test.cpp)
add_executable(diffuse_test src/diffuse_test.cpp)
add_executable(material_test src/material_test.cpp)
add_executable(dielectric_test src/dielectric_test.cpp)
add_executable(camera_test src/camera_test.cpp)
add_executable(focus_test src/focus_test.cpp)
add_executable(final_test src/final_test.cpp)
