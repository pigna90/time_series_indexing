cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(time_series_indexing)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_HOME_DIRECTORY}/src)

# SDSL flags
set(GCC_SDSL_FLAGS "-lsdsl -ldivsufsort -ldivsufsort64")
add_definitions(${GCC_SDSL_FLAGS})

# Google Test flags
set(GCC_GTEST_FLAGS "-lgtest")
add_definitions(${GCC_GTEST_FLAGS})

# Build index
SET(exec build_index)
ADD_EXECUTABLE(${exec} src/Baseline.hpp src/Index1.hpp src/build_index.cpp)
TARGET_LINK_LIBRARIES(${exec} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec} PROPERTY COMPILE_DEFINITIONS)

# Run queries
SET(exec run_queries)
ADD_EXECUTABLE(${exec} src/Baseline.hpp src/Index1.hpp src/run_queries.cpp)
TARGET_LINK_LIBRARIES(${exec} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec} PROPERTY COMPILE_DEFINITIONS)

# Build query sets
SET(exec build_query_sets)
ADD_EXECUTABLE(${exec} src/Baseline.hpp src/Index1.hpp src/build_query_sets.cpp)
TARGET_LINK_LIBRARIES(${exec} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec} PROPERTY COMPILE_DEFINITIONS)

# Uint test for baseline
SET(exec test_baseline)
ADD_EXECUTABLE(${exec} src/Baseline.hpp src/Index1.hpp test_cases/test_common.hpp test_cases/test_baseline.cpp)
TARGET_LINK_LIBRARIES(${exec} gtest sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec} PROPERTY COMPILE_DEFINITIONS)

# Unit test for Index1
SET(exec test_index1)
ADD_EXECUTABLE(${exec} src/Baseline.hpp src/Index1.hpp test_cases/test_common.hpp test_cases/test_index1.cpp)
TARGET_LINK_LIBRARIES(${exec} gtest sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec} PROPERTY COMPILE_DEFINITIONS)
