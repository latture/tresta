cmake_minimum_required(VERSION 2.8.11)
project(tresta)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(EXT_BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ext/boost_1_59_0)
set(EXT_EIGEN_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen-3.2.5)
set(EXT_RAPIDJSON_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ext/rapidjson/include)
set(TRESTA_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TRESTA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${EXT_BOOST_ROOT}
                    ${EXT_EIGEN_ROOT}
                    ${EXT_RAPIDJSON_ROOT})

add_library(boostlib ${EXT_BOOST_ROOT}/libs/smart_ptr/src/sp_collector.cpp
                     ${EXT_BOOST_ROOT}/libs/smart_ptr/src/sp_debug_hooks.cpp)

set(CMAKE_PREFIX_PATH "/Users/ryan/Qt/5.5/clang_64/" CACHE STRING "Path to Qt.")
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

set(tresta_headers ${TRESTA_INCLUDE}/containers.h
                   ${TRESTA_INCLUDE}/cylinder.h
                   ${TRESTA_INCLUDE}/sphere.h
                   ${TRESTA_INCLUDE}/abstract_scene.h
                   ${TRESTA_INCLUDE}/truss_scene.h
                   ${TRESTA_INCLUDE}/window.h
                   ${TRESTA_INCLUDE}/mainwindow.h
                   ${TRESTA_INCLUDE}/csv_parser.h
                   ${TRESTA_INCLUDE}/glassert.h
                   ${TRESTA_INCLUDE}/setup.h)

set(tresta_sources ${TRESTA_SRC}/cylinder.cpp
                   ${TRESTA_SRC}/setup.cpp
                   ${TRESTA_SRC}/sphere.cpp
                   ${TRESTA_SRC}/truss_scene.cpp
                   ${TRESTA_SRC}/window.cpp
                   ${TRESTA_SRC}/mainwindow.cpp)

add_subdirectory(src)
