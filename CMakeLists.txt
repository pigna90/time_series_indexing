cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(time_series_indexing)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_BUILD_TYPE Release)

# SDSL flags
set(GCC_SDSL_FLAGS "-lsdsl -ldivsufsort -ldivsufsort64")
add_definitions(${GCC_SDSL_FLAGS})

# Google Test flags
set(GCC_GTEST_FLAGS "-lgtest")
add_definitions(${GCC_GTEST_FLAGS})

# Build index
SET(exec_1 build_index)
ADD_EXECUTABLE(${exec_1} src/Baseline.hpp src/Index1.hpp src/build_index.cpp)
TARGET_LINK_LIBRARIES(${exec_1} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_1} PROPERTY COMPILE_DEFINITIONS)

# Run queries
SET(exec_2 run_queries)
ADD_EXECUTABLE(${exec_2} src/Baseline.hpp src/Index1.hpp src/run_queries.cpp)
TARGET_LINK_LIBRARIES(${exec_2} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_2} PROPERTY COMPILE_DEFINITIONS)

# Build query sets
SET(exec_3 build_query_sets)
ADD_EXECUTABLE(${exec_3} src/Baseline.hpp src/Index1.hpp src/build_query_sets.cpp)
TARGET_LINK_LIBRARIES(${exec_3} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_3} PROPERTY COMPILE_DEFINITIONS)

# Uint test
SET(exec_4 test_baseline)
ADD_EXECUTABLE(${exec_4} src/Baseline.hpp src/Index1.hpp src/test_common.hpp src/test_baseline.cpp)
TARGET_LINK_LIBRARIES(${exec_4} gtest sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_4} PROPERTY COMPILE_DEFINITIONS)
