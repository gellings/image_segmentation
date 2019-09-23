cmake_minimum_required(VERSION 2.8.12)

project(image_mesh)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(image_mesh main.cpp
                     segment-image.h
                     segment-graph.h
                     misc.h
                     imutil.h
                     imconv.h
                     image.h
                     filter.h
                     disjoint-set.h
                     convolve.h)

target_link_libraries(image_mesh ${OpenCV_LIBS})
