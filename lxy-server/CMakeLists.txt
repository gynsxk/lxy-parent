# CMakeList.txt: lxy-server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。

# 引入boost
# https://cmake.org/cmake/help/v3.8/module/FindBoost.html

cmake_minimum_required (VERSION 3.8)

 MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}.")

set(BOOST_ROOT "D:/local/boost_1_74_0")
set(Boost_NO_SYSTEM_PATHS ON)            # 只搜索上语句设置的搜索路径

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

#set(Boost_DEBUG ON)

#set(Boost_COMPILER "-vc142")
#set(Boost_ARCHITECTURE "-x64")

if (${CMAKE_BUILD_TYPE} STREQUAL  "Debug")
    set(Boost_USE_DEBUG_LIBS ON)
    set(Boost_USE_RELEASE_LIBS       OFF)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL  "Release")
    set(Boost_USE_DEBUG_LIBS OFF)
    set(Boost_USE_RELEASE_LIBS  ON)
endif()


find_package(Boost 1.74 COMPONENTS filesystem date_time thread regex REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    

    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")

    add_executable (daytime_cli "daytime_client1.cpp" "lxy-server.h" )
    target_link_libraries (daytime_cli  ${Boost_LIBRARIES})

    add_executable (daytime_srv "daytime_server1.cpp" "lxy-server.h" )
    target_link_libraries (daytime_srv  ${Boost_LIBRARIES})

    add_executable (lxy-server "lxy-server.cpp" "lxy-server.h")
    target_link_libraries (lxy-server  ${Boost_LIBRARIES})
endif()
