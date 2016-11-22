cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
project(time_series_indexing)
file(MAKE_DIRECTORY "bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

# Serialization test main
set(SOURCES src/Baseline.hpp src/build_index.cpp)
add_executable(build_index ${SOURCES})
target_compile_features(build_index PRIVATE cxx_range_for)

set(SOURCES src/build_query_sets.cpp)
add_executable(build_query_sets ${SOURCES})
target_compile_features(build_query_sets PRIVATE cxx_range_for)

set(SOURCES src/Baseline.hpp src/run_queries.cpp)
add_executable(run_queries ${SOURCES})
target_compile_features(run_queries PRIVATE cxx_range_for)
