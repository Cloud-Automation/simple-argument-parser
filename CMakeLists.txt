cmake_minimum_required (VERSION 2.8.7)
project (SimpleArgumentParser C CXX)

set (SimpleArgumentParser_VERSION_MAJOR 1)
set (SimpleArgumentParser_VERSION_MINOR 0)
set (SimpleArgumentParser_VERSION_PATCH 6)

option(WITH_TESTS "Compile with test cases." "OFF")
option(SHARED "Compile shared/static library." "OFF")

IF (SHARED) 
  add_library(sap SHARED "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")
ELSE()
  add_library(sap STATIC "${CMAKE_SOURCE_DIR}/src/sap.c" "${CMAKE_SOURCE_DIR}/src/sap.h")
ENDIF()

set(CMAKE_C_FLAGS "--std=gnu99" ${CMAKE_C_FLAGS})

option(WITH-TESTS "Compile the test application." OFF)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(example-01 
	${CMAKE_SOURCE_DIR}/examples/example_01.c 
	${CMAKE_SOURCE_DIR}/src/sap.c 
	${CMAKE_SOURCE_DIR}/src/sap.h)

IF(WITH_TESTS)
    include(ExternalProject)
    include(thirdparty/googletest.cmake)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

install (TARGETS sap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES ${PROJECT_SOURCE_DIR}/src/sap.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
