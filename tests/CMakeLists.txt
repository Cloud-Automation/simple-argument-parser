include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE TEST_APP_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

add_executable(tests ${TEST_APP_SOURCES})

set_target_properties(tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(tests gtest gtest_main sap pthread)

