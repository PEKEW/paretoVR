
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.10)
project(demo)
set(EIGEN_DIR "C:/Users/pekew/Desktop/drs/DrStudy/games101/coding/eigen3")
set(OpenCV_DIR "C:/Users/pekew/Desktop/demo/opencv/build")

find_package(OpenCV REQUIRED)
include_directories(${EIGEN_DIR})
add_executable(demo main.cpp shape.cpp)
target_link_libraries(demo ${OpenCV_LIBS})
