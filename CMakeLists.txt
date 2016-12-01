cmake_minimum_required (VERSION 3.1.0)
project (SimpleArgumentParser C CXX)

set (SimpleArgumentParser_VERSION_MAJOR 0)
set (SimpleArgumentParser_VERSION_MINOR 1)

add_library(sap SHARED "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")

option(WITH-TESTS "Compile the test application." OFF)

IF(WITH-TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

install (TARGETS sap DESTINATION /usr/lib)
install (FILES ${PROJECT_SOURCE_DIR}/src/sap.h DESTINATION /usr/include)
