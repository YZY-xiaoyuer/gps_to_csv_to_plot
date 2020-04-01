
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(csv_test_project)
set(OpenCV_DIR /home/xiaoyuer/myopencv/opencv)

find_package(OpenCV REQUIRED)

add_executable(csv src/csv.cpp)

target_link_libraries(csv PRIVATE ${OpenCV_LIBS})

