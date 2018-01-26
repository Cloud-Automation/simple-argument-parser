set(GTEST_PREFIX      gtest)
set(GTEST_VERSION     release-1.8.0)
set(GTEST_BIN_DIR     ${PROJECT_BINARY_DIR}/${GTEST_PREFIX}-build)
set(GTEST_INSTALL_DIR ${PROJECT_BINARY_DIR}/${GTEST_PREFIX}-install)
set(GTEST_SRC_DIR     ${GTEST_INSTALL_DIR})
set(GTEST_INC_DIR     ${GTEST_INSTALL_DIR}/include)
set(GTEST_LIB_DIR     ${GTEST_INSTALL_DIR}/lib)

ExternalProject_Add(${GTEST_PREFIX}
    PREFIX ${GTEST_PREFIX}
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG ${GTEST_VERSION}
    BINARY_DIR ${GTEST_BIN_DIR}
    CMAKE_ARGS 
      -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR} 
    LOG_DOWNLOAD 1
    LOG_BUILD 1
    STEP_TARGETS build)

set(GTEST_LIB ${GTEST_LIB_DIR}/libgtest.a)
set(GMOCK_LIB ${GTEST_LIB_DIR}/libgmock.a)
set(GTEST_LIB_MAIN ${GTEST_LIB_DIR}/libgtest_main.a)
set(GMOCK_LIB_MAIN ${GTEST_LIB_DIR}/libgmock_main.a)

message(STATUS "Source directory of ${GTEST_PREFIX} ${GTEST_SRC_DIR}")
message(STATUS "Source directory of ${GTEST_PREFIX} ${GTEST_INC_DIR}")
message(STATUS "Build directory of ${GTEST_PREFIX} ${GTEST_BIN_DIR}")
