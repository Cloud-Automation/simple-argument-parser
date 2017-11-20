cmake_minimum_required (VERSION 2.8.7)
project (SimpleArgumentParser C CXX)

set (SimpleArgumentParser_VERSION_MAJOR 0)
set (SimpleArgumentParser_VERSION_MINOR 1)

option(WITH-TESTS "Compile with test cases." OFF)
option(STATIC "Compile shared/static library." "SHARED")

IF (STATIC) 
  add_library(sap STATIC "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")
ELSE()
  add_library(sap SHARED "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")
ENDIF()

set(CMAKE_C_FLAGS "--std=gnu99" ${CMAKE_C_FLAGS})

option(WITH-TESTS "Compile the test application." OFF)

IF(WITH-TESTS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

install (TARGETS sap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES ${PROJECT_SOURCE_DIR}/src/sap.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
