cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
project(time_series_indexing)
file(MAKE_DIRECTORY "bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

# Serialization test main
set(SOURCES src/Baseline.hpp src/build_index.cpp)
add_executable(build_index ${SOURCES})
target_compile_features(build_index PRIVATE cxx_range_for)
