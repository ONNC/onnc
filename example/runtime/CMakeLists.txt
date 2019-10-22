cmake_minimum_required(VERSION 2.8.9)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MKLDNN_LIBRARY_PATH /usr/local/lib64)

add_subdirectory(src)
