
cmake_minimum_required(VERSION 3.1)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(plot_test_project)

set(OpenCV_DIR /home/xiaoyuer/myopencv/opencv)

find_package(OpenCV REQUIRED)

add_executable(plot src/plot.cpp)

target_link_libraries(plot PRIVATE ${OpenCV_LIBS})

