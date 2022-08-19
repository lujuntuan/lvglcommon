cmake_minimum_required(VERSION 3.8)
project(lvglcommon LANGUAGES C)

message(STATUS " ")
message(STATUS "CMAKE Version = ${CMAKE_VERSION}")
message(STATUS " ")
message(STATUS "*")
message(STATUS "----------------------------------------------------------------")
message(STATUS "                       CMAKE LittleVGL Project...")
message(STATUS "----------------------------------------------------------------")
message(STATUS "*")

get_filename_component(_SOURCE_ROOT "${PROJECT_SOURCE_DIR}" REALPATH)
set(PROJECT_SOURCE_DIR ${_SOURCE_ROOT})
set(CMAKE_SOURCE_DIR ${_SOURCE_ROOT})

include(${PROJECT_SOURCE_DIR}/cmake/platform.cmake)

message(STATUS "System = ${PL_SYS}, Compiler = ${PL_COMPILER}, Arch = ${PL_ARCH}")

include(${PROJECT_SOURCE_DIR}/cmake/common.cmake)

message(STATUS "*")
message(STATUS "LVGL_3RDPARTY_INCLUDE_DIR = ${LVGL_3RDPARTY_INCLUDE_DIR}")
message(STATUS "LVGL_3RDPARTY_LIB_DIR = ${LVGL_3RDPARTY_LIB_DIR}")
message(STATUS " ")

include(${PROJECT_SOURCE_DIR}/cmake/lvgl.cmake)

if(EXISTS ${PROJECT_SOURCE_DIR}/examples/)
    option(LVGL_BUILD_EXAMPLE "" ON)
    if(LVGL_BUILD_EXAMPLE)
        include(${PROJECT_SOURCE_DIR}/cmake/examples.cmake)
    endif()
endif()
