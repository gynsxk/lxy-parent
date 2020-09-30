# CMakeList.txt: lxy-server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。

# 引入boost
# https://cmake.org/cmake/help/v3.8/module/FindBoost.html

cmake_minimum_required (VERSION 3.8)

MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}.")

# set(OpenCV_DIR "D:/local/opencv411/opencv/build")
find_package(OpenCV REQUIRED PATHS "D:/local/opencv411/opencv/build")

include_directories(${OpenCV_INCLUDE_DIRS})
    

add_executable (lxy-opencv "lxy-opencv.cpp" )
target_link_libraries (lxy-opencv  ${OpenCV_LIBS})


