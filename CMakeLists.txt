cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_OSX_ARCHITECTURES arm64)

project(Image-too-ASCII-art VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

# tell the build to include the headers from OpenCV
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(Image-too-ASCII-art main.cpp)

target_link_libraries( Image-too-ASCII-art ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})