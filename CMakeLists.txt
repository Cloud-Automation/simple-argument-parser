cmake_minimum_required (VERSION 3.1.0)
project (SimpleArgumentParser C CXX)

set (SimpleArgumentParser_VERSION_MAJOR 0)
set (SimpleArgumentParser_VERSION_MINOR 1)


add_library(sap SHARED "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")
